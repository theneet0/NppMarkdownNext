const assert = require('node:assert/strict');
const fs = require('node:fs');
const path = require('node:path');
const vm = require('node:vm');
const root = path.resolve(__dirname, '..');
const bundle = fs.readFileSync(path.join(root, 'src/KelkBundle.inc'), 'utf8');
const marked = bundle.split('/* === marked.umd.min_18.0.14.js === */')[1].split('/* === purify.min_3.4.16.js === */')[0];
const generator = fs.readFileSync(path.join(root, 'tools/gen-kelk-bundle.py'), 'utf8');
let controller = generator.match(/controller_js = r"""([\s\S]*?)"""/)[1];
const end = controller.lastIndexOf('})();');
controller = controller.slice(0, end) + 'globalThis.readerTest = {parseWithLines, slugHeading, captureBlockSources, annotateSourceLines};' + controller.slice(end);
const webcrypto = require('node:crypto').webcrypto;
const context = {console, crypto: {getRandomValues: webcrypto.getRandomValues.bind(webcrypto)}, TextEncoder, module: {exports: {}}, window: {addEventListener() {}}, document: {readyState: 'loading', addEventListener() {}}, setTimeout, clearTimeout};
context.exports = context.module.exports;
context.window.READER_CALLOUTS = JSON.parse(fs.readFileSync(path.join(root, 'themes/callouts.json'), 'utf8'));
context.window.READER_HEADING_POLICY = JSON.parse(fs.readFileSync(path.join(root, 'themes/heading-policy.json'), 'utf8'));
vm.runInNewContext(marked, context);
context.marked = context.module.exports;
vm.runInNewContext(controller, context);
const {parseWithLines, slugHeading, captureBlockSources, annotateSourceLines} = context.readerTest;
let passed = 0;
function test(name, fn) { fn(); passed++; process.stdout.write('PASS ' + name + '\n'); }
test('NestedSourceMap', () => {
    const html = parseWithLines('# Heading\n\n> Quote\n>\n> More\n\n- [ ] Task\n  - Nested\n');
    assert.match(html, /<h1 data-line="0"/);
    assert.match(html, /<p data-line="4">More/);
    assert.match(html, /task-checkbox" data-line="6"/);
    assert.match(html, /<li data-line="7"/);
});
test('ContainerSourceCases', () => {
    const html = parseWithLines('[ref]: https://example.com\r\n\r\n# H\r\n\r\nsame\r\n\r\nsame\r\n');
    assert.match(html, /<h1 data-line="2"/);
    assert.match(html, /<p data-line="4">same/);
    assert.match(html, /<p data-line="6">same/);
    assert.match(parseWithLines('>\n> P\n'), /<p data-line="1">P/);
    assert.match(parseWithLines('- [ ] Task\n\n  continuation\n\n- [x] Next\n'), /<p data-line="2">continuation/);
    assert.match(parseWithLines('- item\n\t- nested\n\t\tcontinuation\n'), /<li data-line="1"/);
    assert.match(parseWithLines('> line\nlazy\n>\n> next\n'), /<p data-line="3">next/);
});
test('FixtureSourceMaps', () => {
    for (const name of ['sample_english.md', 'sample_persian.md']) {
        const html = parseWithLines(fs.readFileSync(path.join(root, 'tests/fixtures', name), 'utf8'));
        assert.ok(!html.includes('data-source-unmapped'), name + ' has verified source positions');
    }
});
test('UnknownMappingIsNotInteractive', () => {
    const tokens = [{type: 'paragraph', raw: 'absent', tokens: []}];
    annotateSourceLines(tokens, 'different', [0]);
    assert.equal(tokens[0].sourceLine, null);
    const uncertain = [{type:'paragraph',raw:'absent'}, {type:'paragraph',raw:'same'}];
    annotateSourceLines(uncertain, 'same', [0]);
    assert.equal(uncertain[1].sourceLine, null, 'Do not guess positions beyond an unmatched token');
    const code = parseWithLines('```text\n- [ ] Task\n```\n');
    assert.ok(!code.includes('class="task-checkbox"'));
});
test('GeneratedTasksHavePrivateKeys', () => {
    const source = '> - [x] literal [ ]\n>   - [ ] Nested\n';
    const html = parseWithLines(source);
    const keys = [...html.matchAll(/data-task-key="([^"]+)"/g)].map(match => match[1]);
    assert.equal(keys.length, 2);
    assert.equal(new Set(keys).size, 2);
    assert.ok(keys.every(key => /^[0-9a-f]{32}$/.test(key)));
    const second = [...parseWithLines(source).matchAll(/data-task-key="([^"]+)"/g)].map(match => match[1]);
    assert.ok(second.every(key => !keys.includes(key)), 'Raw HTML cannot predict generated keys across renders');
});
test('HeadingCorpus', () => {
    const cases = JSON.parse(fs.readFileSync(path.join(root, 'tests/fixtures/headings.json'), 'utf8'));
    for (const item of cases) {
        const used = new Set(item.reserved);
        const actual = item.headings.map(text => slugHeading(text, used));
        assert.deepEqual(actual, item.expected, item.name);
    }
});
test('ImageHeadingMarkdown', () => {
    const html = parseWithLines('# ![Logo](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///ywAAAAAAQABAAACAUwAOw==)\n');
    assert.match(html, /<h1 data-line="0">/);
    assert.match(html, /<img [^>]*alt="Logo"/);
});
test('CalloutTypesAndLines', () => {
    for (const item of context.window.READER_CALLOUTS) {
        for (const alias of item.aliases) {
            const html = parseWithLines('> [!' + alias + ']\r\n> Body\r\n');
            assert.match(html, new RegExp('alert-callout alert-' + item.type));
            assert.match(html, /<p data-line="1">Body/);
            assert.ok(!html.includes('[!' + alias + ']'));
        }
    }
});
test('CalloutLookalikesUntouched', () => {
    for (const md of ['> ordinary [!NOTE]\n', '> \\[!NOTE]\n', '> `[!NOTE]`\n', '> [!NOTES]\n', '```\n> [!NOTE]\n```\n']) {
        assert.ok(!parseWithLines(md).includes('alert-callout'), md);
    }
    const nested = parseWithLines('> [!NOTE]\n> Body\n>\n> > nested\n');
    assert.match(nested, /<blockquote data-line="3"/);
});
process.stdout.write('All ' + passed + ' parser suites passed.\n');
