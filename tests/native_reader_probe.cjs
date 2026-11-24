// The PowerShell runner verifies ownership of this isolated loopback CDP endpoint.
const fs = require('node:fs');
const path = require('node:path');

function options(argv) {
  const result = {};
  for (let i = 0; i < argv.length; i += 2) {
    if (!argv[i].startsWith('--') || argv[i + 1] === undefined) throw new Error('Expected --name value arguments');
    result[argv[i].slice(2)] = argv[i + 1];
  }
  const port = Number(result.port);
  if (!Number.isInteger(port) || port < 1 || port > 65535) throw new Error('A valid loopback --port is required');
  return {...result, port};
}

class Cdp {
  constructor(socket) {
    this.socket = socket;
    this.pending = new Map();
    this.nextId = 0;
    socket.addEventListener('message', event => {
      let message;
      try { message = JSON.parse(String(event.data)); } catch { return; }
      if (!message.id || !this.pending.has(message.id)) return;
      const request = this.pending.get(message.id);
      this.pending.delete(message.id);
      clearTimeout(request.timer);
      if (message.error) {
        const error = new Error(message.error.message);
        error.code = message.error.code;
        request.reject(error);
      } else request.resolve(message.result || {});
    });
    socket.addEventListener('close', () => {
      for (const request of this.pending.values()) {
        clearTimeout(request.timer);
        if (request.allowDisconnect) request.resolve({disconnected: true});
        else request.reject(new Error('The isolated CDP connection closed'));
      }
      this.pending.clear();
    });
  }

  static async connect(url, port) {
    const parsed = new URL(url);
    if (!['127.0.0.1', 'localhost', '[::1]'].includes(parsed.hostname) || Number(parsed.port) !== port) {
      throw new Error('Refusing a CDP target outside the verified loopback endpoint');
    }
    if (typeof WebSocket !== 'function') throw new Error('Node 24 with built-in WebSocket is required');
    const socket = new WebSocket(url);
    await new Promise((resolve, reject) => {
      const timer = setTimeout(() => { socket.close(); reject(new Error('CDP connection timed out')); }, 5000);
      socket.addEventListener('open', () => { clearTimeout(timer); resolve(); }, {once: true});
      socket.addEventListener('error', () => { clearTimeout(timer); reject(new Error('CDP connection failed')); }, {once: true});
    });
    return new Cdp(socket);
  }

  send(method, params = {}, allowDisconnect = false) {
    const id = ++this.nextId;
    return new Promise((resolve, reject) => {
      const timer = setTimeout(() => {
        this.pending.delete(id);
        if (allowDisconnect) resolve({requestTimedOut: true});
        else reject(new Error(method + ' timed out'));
      }, 5000);
      this.pending.set(id, {resolve, reject, timer, allowDisconnect});
      this.socket.send(JSON.stringify({id, method, params}));
    });
  }

  async evaluate(expression) {
    const response = await this.send('Runtime.evaluate', {expression, returnByValue: true, awaitPromise: true});
    if (response.exceptionDetails) throw new Error(response.exceptionDetails.text || 'Reader expression failed');
    return response.result?.value;
  }

  async close() {
    if (this.socket.readyState >= 2) return;
    await new Promise(resolve => {
      const timer = setTimeout(resolve, 300);
      this.socket.addEventListener('close', () => { clearTimeout(timer); resolve(); }, {once: true});
      this.socket.close();
    });
  }
}

async function json(port, endpoint) {
  const response = await fetch('http://127.0.0.1:' + port + endpoint, {signal: AbortSignal.timeout(5000)});
  if (!response.ok) throw new Error('CDP discovery HTTP ' + response.status);
  return response.json();
}

function readState() {
  const content = document.getElementById('content');
  if (!content || !document.getElementById('theme-fab')) return null;
  const color = selector => {
    const element = content.querySelector(selector);
    return element ? getComputedStyle(element).color : null;
  };
  const index = window.readerDocumentIndex;
  if (typeof window.getReaderScrollRoot !== 'function') throw new Error('Reader scroll-root API is missing');
  const scrollRoot = window.getReaderScrollRoot();
  return {
    title: document.title, text: content.textContent.slice(0, 1048576),
    state: index ? index.state : null, sequence: index ? index.sequence : null,
    bufferId: index ? index.bufferId : null, renderRevision: index ? index.renderRevision : null,
    tasks: [...content.querySelectorAll('input.task-checkbox')].map(input => ({checked:input.checked,disabled:input.disabled})),
    themeId: window._themeId, sync: window._syncEnabled, bidi: window._bidiEnabled,
    autoShow: window._autoShowPanel, zoomPercent: window._zoomPercent,
    fontSizePx: window._fontSizePx, lineHeightPercent: window._lineHeightPercent,
    maxWidthPx: window._maxWidthPx, outline: document.body.classList.contains('toc-open'),
    scrollY: scrollRoot.scrollTop, documentHeight: scrollRoot.scrollHeight,
    viewportHeight: scrollRoot.clientHeight, scrollRoot: scrollRoot.tagName,
    gear: !!document.getElementById('theme-fab'),
    colors: {background: getComputedStyle(document.body).backgroundColor,
      heading: color('h1'), strong: color('p strong'), emphasis: color('p em'),
      code: color('p code'), quote: color('blockquote p'), link: color('a')}
  };
}
const stateExpression = '(' + readState.toString() + ')()';

async function reader(port) {
  const targets = await json(port, '/json/list');
  for (const target of targets) {
    if (target.type !== 'page' || !target.webSocketDebuggerUrl) continue;
    let client;
    try {
      client = await Cdp.connect(target.webSocketDebuggerUrl, port);
      const state = await client.evaluate(stateExpression);
      if (state) return {client, state, target};
    } catch (error) {
      if (client) await client.close();
      continue;
    }
    if (client) await client.close();
  }
  throw new Error('The isolated reader target is not ready');
}

async function main() {
  const args = options(process.argv.slice(2));
  if (args.command === 'crash-browser') {
    const version = await json(args.port, '/json/version');
    const client = await Cdp.connect(version.webSocketDebuggerUrl, args.port);
    try {
      return {command: args.command, requested: true, ...await client.send('Browser.crash', {}, true)};
    } catch (error) {
      if (error.code === -32601) return {command: args.command, unsupported: true, error: error.message};
      throw error;
    } finally { await client.close(); }
  }
  const {client, state, target} = await reader(args.port);
  try {
    if (args.command === 'crash-renderer') {
      return {command: args.command, requested: true, targetId: target.id, ...await client.send('Page.crash', {}, true)};
    }
    if (args.command === 'evaluate') {
      if (!args['expression-base64']) throw new Error('An explicit trusted expression is required');
      return {targetId: target.id, value: await client.evaluate(Buffer.from(args['expression-base64'], 'base64').toString('utf8'))};
    }
    if (!['state', 'inspect'].includes(args.command)) throw new Error('Unknown probe command');
    if (args.command === 'inspect') {
      function inspectMenu() {
        if (typeof openSettingsPopover !== 'function') throw new Error('Native settings entry point is missing');
        openSettingsPopover();
        const labels = [...document.querySelectorAll('#theme-pop-list .theme-pop-item')].map(item => item.textContent.trim());
        closeThemePopover();
        return labels;
      }
      state.settings = await client.evaluate('(' + inspectMenu.toString() + ')()');
    }
    if (args.screenshot) {
      const screenshot = await client.send('Page.captureScreenshot', {format: 'png', captureBeyondViewport: false});
      const destination = path.resolve(args.screenshot);
      fs.mkdirSync(path.dirname(destination), {recursive: true});
      fs.writeFileSync(destination, Buffer.from(screenshot.data, 'base64'));
      state.screenshot = destination;
    }
    return {targetId: target.id, ...state};
  } finally { await client.close(); }
}

main().then(result => {
  process.stdout.write(JSON.stringify(result) + '\n');
}).catch(error => {
  process.stderr.write(JSON.stringify({error: error.message, code: error.code || null}) + '\n');
  process.exitCode = 1;
});
