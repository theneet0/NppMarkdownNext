async () => {
    if (!Array.isArray(window.__messages)) throw new Error('Run against the local mock-host reader fixture');
    const checks = [];
    function expect(condition, description) {
        if (!condition) throw new Error(description);
        checks.push(description);
    }
    function finishAnimations() { document.getAnimations().forEach(animation => animation.finish()); }
    function row(label) {
        const element = [...document.querySelectorAll('#theme-pop-list .theme-pop-item')]
            .find(item => item.textContent.trim().startsWith(label));
        if (!element) throw new Error('Missing menu row: ' + label);
        return element;
    }
    function settings() {
        closeThemePopover();
        document.getElementById('theme-fab').click();
        finishAnimations();
    }
    function themes() { settings(); row('Theme:').click(); }
    function selectTheme(id) {
        themes();
        if (id === 'bearded' || id.startsWith('bearded-v-')) {
            document.querySelector('#theme-pop-list [data-theme="bearded"]').click();
        }
        const element = document.querySelector('#theme-pop-list [data-theme="' + id + '"]');
        if (!element) throw new Error('Missing theme: ' + id);
        element.click();
        finishAnimations();
    }
    settings();
    expect(document.querySelectorAll('#theme-pop-list .theme-pop-item').length === 14, 'All settings including Hide editor, Reading width, Typography and Language are present');
    expect(document.getElementById('theme-fab').getAttribute('aria-expanded') === 'true', 'Gear opens the settings menu');
    const originalText = document.getElementById('content').textContent;
    const options = [...THEME_LIST, ...window.BEARDED_VARIANTS];
    const normalizer = document.createElement('span');
    normalizer.style.display = 'none';
    document.body.appendChild(normalizer);
    for (const option of options) {
        selectTheme(option.id);
        const css = getComputedStyle(document.body);
        normalizer.style.color = css.getPropertyValue('--bg-page');
        expect(css.backgroundColor === getComputedStyle(normalizer).color, 'Palette applies: ' + option.id);
        expect(window._themeId === option.id, 'Theme selection persists: ' + option.id);
        expect(document.getElementById('content').textContent === originalText, 'Theme preserves Markdown: ' + option.id);
    }
    normalizer.remove();
    selectTheme('light');
    const lightClass = document.body.className;
    const messageCount = window.__messages.length;
    themes();
    document.querySelector('#theme-pop-list [data-theme="dark"]').dispatchEvent(new MouseEvent('mouseover', {bubbles: true}));
    finishAnimations();
    expect(document.body.classList.contains('dark'), 'Hover previews a different theme');
    expect(window._themeId === 'light' && window.__messages.length === messageCount, 'Hover does not persist a theme');
    document.getElementById('theme-popover').dispatchEvent(new MouseEvent('mouseleave'));
    finishAnimations();
    expect(document.body.className === lightClass, 'Leaving the theme menu restores the selected theme');
    for (const [label, field] of [['Sync with caret', '_syncEnabled'], ['Smart BiDi', '_bidiEnabled'], ['Auto-show for Markdown', '_autoShowPanel']]) {
        settings(); row(label).click();
        expect(window[field] === false && row(label).getAttribute('aria-pressed') === 'false', label + ' toggles off');
        row(label).click();
        expect(window[field] === true, label + ' toggles back on');
    }
    settings(); row('Outline / TOC').click();
    expect(document.body.classList.contains('toc-open') && !document.getElementById('toc-drawer').inert, 'Outline opens');
    expect(document.querySelectorAll('#toc-drawer .toc-item').length === 2, 'Outline lists the actual headings');
    document.querySelector('.toc-close').click();
    expect(!document.body.classList.contains('toc-open') && document.getElementById('toc-drawer').inert, 'Outline closes');
    settings(); row('Zoom:').click(); row('125%').click();
    expect(window._zoomPercent === 125, 'Zoom preset reaches the host');
    row('Zoom in').click(); expect(window._zoomPercent === 135, 'Zoom in updates the menu');
    row('Zoom out').click(); expect(window._zoomPercent === 125, 'Zoom out updates the menu');
    row('Reset (100%)').click(); expect(window._zoomPercent === 100, 'Zoom resets');
    selectTheme('follow');
    settings(); row('Dark mode:').click(); row('Light').click();
    expect(window._darkMode === 0, 'Dark mode override reaches the host');
    settings(); row('Dark mode:').click(); row('Auto (follow').click();
    expect(window._darkMode === -1, 'Dark mode can follow Notepad++ again');
    function resultFor(command, status, detail = '') {
        const operation = command.startsWith('copyHtml:') ? 'copyHtml' : 'saveAsHtml';
        window.__emit({type:'operationResult',operation,status,detail,error:status==='failed'?5:0,recoveryPath:'',requestId:command.split(String.fromCharCode(10))[0].slice(operation.length+1)});
    }
    settings(); row('Copy HTML').click();
    const copy = window.__messages.findLast(message => message.startsWith('copyHtml:'));
    expect(!!copy, 'Copy HTML reaches the host with an operation ID');
    resultFor(copy, 'succeeded');
    settings(); row('Save HTML...').click();
    const save = window.__messages.findLast(message => message.startsWith('saveAsHtml:'));
    expect(!!save, 'Save HTML reaches the host with an operation ID');
    resultFor(save, 'cancelled');
    settings(); row('About NppMarkdownNext').click();
    expect(document.getElementById('theme-pop-list').textContent.includes('v1.6.0'), 'About shows the current version');
    expect(document.querySelectorAll('#content > ol > li').length === 10, 'Numbering has ten top-level items');
    expect(document.querySelectorAll('#content ol ul').length === 2, 'Both nested lists stay inside the ordered list');
    selectTheme('bearded-v-gold-d-raynh');
    const gold = getComputedStyle(document.body).backgroundColor;
    expect(gold === 'rgb(15, 22, 40)', 'Bearded Gold retains its original background');
    async function settled() { await new Promise(requestAnimationFrame); await new Promise(requestAnimationFrame); finishAnimations(); }
    const scrollRoot = () => window.getReaderScrollRoot();
    const scrollTo = top => { scrollRoot().scrollTop = top; };
    let nativeRevision = 1;
    async function render(markdown, bufferId = '11', caretLine = -1) {
        const theme = window.READER_THEME_CATALOG.find(item => item.id === window._themeId);
        window.__emit({type:'render',markdown,bufferId,caretLine,renderRevision:String(++nativeRevision),title:'Regression',isDark:theme ? theme.isDark : true,isBiDi:true,themeId:window._themeId});
        await settled();
    }
    const color = selector => getComputedStyle(document.querySelector(selector)).color;
    await render('# Gold roles\n\n**Bold** *Italic* `inline`\n\n> Quote\n\n[link](https://example.com)\n\n```python\n# author comment\n```\n');
    for (const [selector, expected] of [['h1','rgb(255, 208, 0)'],['strong','rgb(237, 242, 247)'],['em','rgb(255, 130, 63)'],[':not(pre) > code','rgb(161, 103, 255)'],['blockquote','rgb(228, 172, 115)'],['a','rgb(62, 178, 255)'],['.hljs-comment','rgb(113, 134, 192)']]) {
        expect(color('#content ' + selector) === expected, 'GoldMarkdownRoles ' + selector);
    }
    expect(color('#content') === 'rgb(184, 196, 228)', 'GoldMarkdownRoles body text');
    const tablesMd = '| a | b |\n|---|---|\n| 1 | 2 |\n\n| id | where |\n|---|---|\n| x | `' + 'w'.repeat(140) + '` |\n';
    await render(tablesMd, '31');
    const content = document.getElementById('content'), contentBox = content.getBoundingClientRect();
    const [smallWrap, wideWrap] = content.querySelectorAll('.reader-table-wrap');
    expect(smallWrap && wideWrap && smallWrap.querySelectorAll('.reader-col-grip').length === 2, 'Tables are wrapped and get one resize grip per column');
    expect(Math.abs(smallWrap.firstChild.getBoundingClientRect().left - contentBox.left - parseFloat(getComputedStyle(content).paddingLeft)) < 2, 'A narrow table stays aligned with the reading column');
    expect(wideWrap.firstChild.getBoundingClientRect().width <= content.clientWidth + 1 || wideWrap.classList.contains('reader-table-overflow'), 'A wide table follows the reading column width or scrolls inside it');
    const firstTable = () => content.querySelector('.reader-table-wrap table');
    const smallFp = tableFingerprint(firstTable(), {});
    receiveTableWidths('31', smallFp + ':120,200;abc:1');
    expect(firstTable().rows[0].cells[1].style.width === '200px', 'Saved widths from the host apply to the shown document at once, malformed entries are skipped');
    window.__messages.length = 0;
    firstTable().querySelector('.reader-col-grip').dispatchEvent(new MouseEvent('dblclick', {bubbles: true}));
    expect(firstTable().rows[0].cells[1].style.width === '' && window.__messages.includes('tableWidths:31:'), 'Resetting a column sends the updated widths to the host');
    receiveTableWidths('31', smallFp + ':120,200');
    await render('| new | table |\n|---|---|\n| 0 | 0 |\n\n' + tablesMd, '31');
    const moved = content.querySelectorAll('.reader-table-wrap table')[1];
    expect(moved.rows[0].cells[1].style.width === '200px' && content.querySelector('.reader-table-wrap table').rows[0].cells[1].style.width === '', 'Widths follow their table when another table is inserted above');
    await render(tablesMd, '32');
    expect(firstTable().rows[0].cells[1].style.width === '', 'Resized column widths do not leak into another buffer');

    // Reader tools: context menu, copy helpers, code buttons, outline button, warnings, links
    await render('Para [site](https://example.com/x) and [plan](notes/plan.md).\n\n| h1 | h2 |\n|---|---|\n| a\tb | c |\n\n```js\nlet x = 1;\n```\n', '33');
    const menu = document.getElementById('reader-menu');
    const menuLabels = () => [...menu.querySelectorAll('.reader-menu-item')].map(item => item.firstChild.textContent);
    content.querySelector('td').dispatchEvent(new MouseEvent('contextmenu', {bubbles: true, cancelable: true, clientX: 40, clientY: 40}));
    expect(!menu.hidden && menuLabels().includes('Copy table') && !menuLabels().includes('Copy code') && menu.querySelector('.reader-menu-item').disabled, 'Context menu offers table actions; Copy is disabled without a selection');
    document.dispatchEvent(new KeyboardEvent('keydown', {key: 'Escape', bubbles: true}));
    expect(menu.hidden, 'Escape closes the context menu');
    content.querySelector('pre code').dispatchEvent(new MouseEvent('contextmenu', {bubbles: true, cancelable: true, clientX: 9999, clientY: 9999}));
    const menuBox = menu.getBoundingClientRect();
    expect(menuLabels().includes('Copy code') && menuBox.right <= window.innerWidth && menuBox.bottom <= window.innerHeight, 'Context menu offers code actions and stays inside the pane');
    window.__messages.length = 0;
    [...menu.querySelectorAll('.reader-menu-item')].find(item => item.textContent.startsWith('Find in preview')).click();
    expect(menu.hidden && window.__messages.includes('find'), 'Find in preview asks the host to open its find bar');
    const tableData = tableClipboard(content.querySelector('table'));
    expect(tableData.text === 'h1\th2\na b\tc' && !tableData.html.includes('reader-col-grip') && !tableData.html.includes('data-line'), 'Copy table yields clean HTML and tab-separated text');
    expect(content.querySelectorAll('pre > .reader-copy-code').length === 1 && codeText(content.querySelector('pre')) === 'let x = 1;', 'Code blocks get one copy button that copies only the code');
    window.__messages.length = 0;
    content.querySelector('a[href="notes/plan.md"]').click();
    content.querySelector('a[href^="https:"]').click();
    expect(window.__messages.join('|') === 'openLink:notes/plan.md|openLink:https://example.com/x', 'Links are routed to the host instead of navigating the preview');
    document.getElementById('toc-fab').click();
    expect(document.body.classList.contains('toc-open'), 'Outline button opens the outline');
    document.getElementById('toc-fab').click();
    expect(!document.body.classList.contains('toc-open'), 'Outline button closes the outline');
    const editorFab = document.getElementById('editor-fab');
    window.__messages.length = 0;
    editorFab.click();
    expect(document.body.classList.contains('editor-hidden') && editorFab.getAttribute('aria-pressed') === 'true' && window.__messages.includes('toggleHideEditor'), 'Editor FAB enables Hide editor mode and notifies the host');
    expect(editorFab.title === window.READER_STRINGS.en['ui.showEditorButton'], 'Editor FAB tooltip switches to Show editor when active');
    settings();
    expect(row('Hide editor').getAttribute('aria-pressed') === 'true', 'Settings reflects Hide editor state');
    row('Hide editor').click();
    expect(!document.body.classList.contains('editor-hidden') && editorFab.getAttribute('aria-pressed') === 'false', 'Settings Hide editor row restores editor visibility');
    closeThemePopover();
    setReaderWarning('reader-operation-warning', 'Test warning');
    const warning = document.getElementById('reader-operation-warning');
    expect(!warning.hidden && getComputedStyle(warning).pointerEvents === 'auto', 'Warnings accept the pointer');
    warning.querySelector('.reader-warning-close').click();
    expect(warning.hidden, 'Warnings can be dismissed');

    // Sections, row filters, width resets, keyboard grips, back after a jump, outline filter and width, missing images
    await render('# Report\n\nIntro\n\n## Findings\n\nAbout [details](#details).\n\n| ID | Sev |\n|---|---|\n| A1 | High |\n| A2 | Low |\n| A3 | High |\n\n### Details\n\nDeep text.\n\n## Next\n\n![chart](img/chart.png)\n\n![abs](C:\\tmp\\x.png)\n', '36');
    const findingsText = content.querySelector('table').closest('.reader-table-wrap').previousElementSibling;
    const section = sectionClipboard(sectionOf(findingsText));
    expect(section.html.includes('<h2') && section.html.includes('Deep text') && !section.html.includes('Next') && !section.html.includes('reader-col-grip') && section.text.startsWith('Findings'), 'Copy section takes the heading up to the next heading of the same level, clean');
    const table36 = content.querySelector('table');
    content.querySelector('td').dispatchEvent(new MouseEvent('contextmenu', {bubbles: true, cancelable: true, clientX: 40, clientY: 40}));
    expect(menuLabels().includes('Filter rows') && menuLabels().includes('Copy section') && !menuLabels().includes('Reset column widths'), 'The table menu offers row filtering and section copy; no reset without widths');
    [...menu.querySelectorAll('.reader-menu-item')].find(item => item.textContent.startsWith('Filter rows')).click();
    const filterInput = content.querySelector('.reader-table-filter input');
    filterInput.value = 'high'; filterInput.dispatchEvent(new Event('input'));
    expect([...table36.tBodies[0].rows].filter(row => !row.hidden).length === 2 && content.querySelector('.reader-table-filter-count').textContent === '2 / 3', 'The row filter shows matching rows and a count');
    expect(tableClipboard(table36).text.split('\n').length === 4, 'Copy table still copies every row while filtered');
    await render(content.textContent ? '# Report\n\nIntro\n\n## Findings\n\nAbout [details](#details).\n\n| ID | Sev |\n|---|---|\n| A1 | High |\n| A2 | Low |\n| A3 | High |\n\n### Details\n\nDeep text.\n' : '', '36');
    expect(content.querySelector('.reader-table-filter input').value === 'high' && [...content.querySelector('table').tBodies[0].rows].filter(row => !row.hidden).length === 2, 'The row filter survives a re-render');
    const grip36 = content.querySelector('table .reader-col-grip');
    const before36 = content.querySelector('table').rows[0].cells[0].getBoundingClientRect().width;
    grip36.dispatchEvent(new KeyboardEvent('keydown', {key: 'ArrowRight', shiftKey: true, bubbles: true}));
    expect(grip36.tabIndex === 0 && Math.abs(content.querySelector('table').rows[0].cells[0].getBoundingClientRect().width - before36 - 64) < 3, 'Shift+Right on a focused grip widens the column by 64px');
    content.querySelector('td').dispatchEvent(new MouseEvent('contextmenu', {bubbles: true, cancelable: true, clientX: 40, clientY: 40}));
    expect(menuLabels().includes('Reset column widths') && menuLabels().includes('Reset all column widths'), 'Width resets appear once a table has widths');
    window.__messages.length = 0;
    [...menu.querySelectorAll('.reader-menu-item')].find(item => item.textContent.startsWith('Reset all column widths')).click();
    expect(content.querySelector('table').style.width === '' && window.__messages.includes('tableWidths:36:'), 'Reset all column widths clears the document and tells the host');
    content.querySelector('.reader-table-filter .reader-warning-close').click();
    scrollTo(0);
    content.querySelector('a[href="#details"]').click();
    expect(!document.getElementById('reader-back').hidden, 'Following an anchor offers Back');
    goBackInDocument();
    expect(scrollRoot().scrollTop === 0 && document.getElementById('reader-back').hidden, 'Back returns to the place before the jump');
    openToc();
    const tocFilter = document.getElementById('toc-filter');
    tocFilter.value = 'det'; tocFilter.dispatchEvent(new Event('input'));
    expect([...document.querySelectorAll('#toc-drawer .toc-item')].filter(link => !link.hidden).map(link => link.textContent).join() === 'Details', 'The outline filter keeps matching headings only');
    tocFilter.value = 'zzz'; tocFilter.dispatchEvent(new Event('input'));
    expect(!document.getElementById('toc-filter-empty').hidden, 'The outline says when nothing matches');
    tocFilter.value = ''; tocFilter.dispatchEvent(new Event('input'));
    document.getElementById('toc-resize').dispatchEvent(new KeyboardEvent('keydown', {key: 'ArrowLeft', bubbles: true}));
    await settled();
    expect(Math.abs(document.getElementById('toc-drawer').getBoundingClientRect().width - 304) < 2 && window.__state.outlineWidthPx === 304, 'The outline edge widens the outline and the width is saved');
    window.__emit(Object.assign({}, window.__state, {outlineWidthPx: 280}));
    closeToc();
    await render('![chart](img/chart.png)\n\n![abs](C:\\tmp\\x.png)\n\n![up](../x.png)\n', '37');
    const notes = [...content.querySelectorAll('.reader-image-missing')].map(note => note.textContent);
    expect(notes.length === 3 && notes[0].includes('Save the document') && notes[1].includes('absolute path') && notes[2].includes('Save the document'), 'Pictures that cannot be shown say which file and why ' + JSON.stringify(notes));

    // Save/Copy HTML send what the reader shows: clean body, contents list for 3+ headings, then the plain text
    await render('# One\n\n## Two\n\n## Three\n\n| a | b |\n|---|---|\n| 1 | 2 |\n\n![net](https://example.com/n.png)\n\nText[^1]\n\n[^1]: Note\n', '38');
    const payload = exportPayload();
    const lengthEnd = payload.indexOf('\n', 1), htmlLength = Number(payload.slice(1, lengthEnd));
    const exportedHtml = payload.slice(lengthEnd + 1, lengthEnd + 1 + htmlLength), exportedText = payload.slice(lengthEnd + 1 + htmlLength);
    expect(payload.charAt(0) === '\n' && exportedHtml.startsWith('<nav class="export-toc"') && exportedHtml.includes('class="footnotes"'), 'The export body is the rendered document with a contents list and footnotes');
    expect(!/reader-col-grip|reader-table-wrap|data-line|reader-remote-image|reader-copy-code/.test(exportedHtml) && exportedHtml.includes('src="https://example.com/n.png"'), 'The export body carries no reader decorations and keeps remote picture addresses');
    expect(exportedText.startsWith('One') && exportedText.split('Two').length === 2 && exportedText.includes('Note'), 'The export plain text follows the document (headings once)');
    expect(!performance.getEntriesByType('resource').some(entry => entry.name.includes('example.com/n.png')), 'Exporting a page with an unclicked web picture fetches nothing');
    const crafted = document.createElement('div');
    crafted.innerHTML = '<a href="tel:123">t</a><a href="notes.md#x">n</a><a href="https://e.com">w</a><img data-remote-src="file:///C:/secret.png"><img data-remote-src="//cdn.e.com/p.png">';
    const cleaned = cleanReaderClone(crafted).innerHTML;
    expect(!cleaned.includes('tel:') && cleaned.includes('href="notes.md#x"') && cleaned.includes('href="https://e.com"') && !cleaned.includes('file:') && cleaned.includes('src="//cdn.e.com/p.png"'), 'Copies keep only safe links and web picture addresses');
    const tableSection = sectionClipboard(sectionOf(content.querySelector('table')));
    expect(!tableSection.html.includes('reader-table-wrap') && tableSection.html.includes('<table'), 'A copied section holds its tables without the reader wrapper');
    expect(rt('toast.sectionMissing', 'a$&b') === window.READER_STRINGS.en['toast.sectionMissing'].replace('{0}', function() { return 'a$&b'; }), 'Arguments with $ stay literal text');
    window.__messages.length = 0;
    settings(); row('Copy HTML').click();
    const copyMessage = window.__messages.find(message => message.startsWith('copyHtml:'));
    expect(copyMessage && copyMessage.indexOf('\n') > 0 && copyMessage.includes('export-toc'), 'Copy HTML sends the rendered content with its request');
    resultFor(copyMessage.split('\n')[0], 'succeeded');

    // Shortcuts follow the typed letter, and the physical key only where the layout types no Latin letter
    expect(shortcutLetter({key: 'а', code: 'KeyF'}, 'f') && shortcutLetter({key: 'f', code: 'KeyY'}, 'f') && !shortcutLetter({key: 'u', code: 'KeyF'}, 'f') && !shortcutLetter({key: 'l', code: 'KeyP'}, 'p'), 'Ctrl+F/Ctrl+P work on Russian, Dvorak and AZERTY layouts');
    // At most the 60 most recently resized tables of a document are sent to the host
    window.__messages.length = 0;
    for (let i = 0; i < 70; i++) readerColumnWidths.set('77|' + ('0000000' + i.toString(16)).slice(-8), [100, 100]);
    persistTableWidths('77|00000000');
    const sentWidths = window.__messages.find(message => message.startsWith('tableWidths:77:')).split(':').slice(2).join(':').split(';');
    expect(sentWidths.length === 60 && sentWidths[0].startsWith('0000000a:') && sentWidths[59].startsWith('00000045:'), 'Width lists are capped to the most recent tables');
    Array.from(readerColumnWidths.keys()).forEach(key => { if (key.startsWith('77|')) readerColumnWidths.delete(key); });
    // Relative images resolve inside the document's folder only
    const base = 'https://npp-reader-document.local/';
    expect(documentRelativeUrl('img/a b.png', base) === base + 'img/a%20b.png' && documentRelativeUrl('./x/../y.png', base) === base + 'y.png', 'Relative image paths map into the document folder');
    expect(['..%2f..%2fx.png', 'a%5Cb.png', 'x/%2e%2e/y.png', '..%20/x.png', '.../x.png', 'x.png:stream', 'dir./x.png', 'con.png', 'img/NUL', 'COM1.png', 'con.tar.png', 'conin$.png', 'CONOUT$', 'com².png', 'lpt¹.png'].every(src => documentRelativeUrl(src, base) === null) && documentRelativeUrl('console.png', base) === base + 'console.png', 'Encoded separators, dot segments and device names cannot leave the folder');
    expect(['../up.png', '/root.png', 'C:\\x.png', 'https://e.com/a.png', 'data:image/png;base64,AA', '//host/a.png', ''].every(src => documentRelativeUrl(src, base) === null) && documentRelativeUrl('a.png', '') === null, 'Absolute, external and escaping paths are left alone');
    await render('<style>@import url(https://e.com/s.css);</style>\n\n<svg><image href="https://e.com/i.png"/></svg>\n\n<span style="background:\\75rl(https://e.com/x.png)">esc</span>\n', '36');
    expect(!content.querySelector('style') && !content.querySelector('svg image[href]') && !content.querySelector('span[style]'), 'Style sheets, SVG references and escaped styles cannot fetch on render');
    // Network images are never fetched on their own: the sanitizer moves the URL aside, a button loads it
    await render('![chart](https://example.com/chart.png)\n\n<img src="//cdn.example.com/p.gif" srcset="https://e.com/a.png 2x" style="background:url(https://e.com/t.png)">\n\n![local](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///ywAAAAAAQABAAACAUwAOw==)\n', '35');
    const remote = [...content.querySelectorAll('img[data-remote-src]')];
    expect(remote.length === 2 && remote.every(img => !img.hasAttribute('src') && !img.hasAttribute('srcset') && !img.hasAttribute('style')) && content.querySelector('img[alt="local"]').getAttribute('src').startsWith('data:'), 'Remote image sources are moved aside before anything is fetched; data images stay');
    const loadButton = content.querySelector('.reader-remote-image');
    expect(content.querySelectorAll('.reader-remote-image').length === 2 && loadButton.textContent.includes('example.com') && loadButton.textContent.includes('chart'), 'Each remote image gets a load button naming its host');
    loadButton.click();
    expect(remote[0].getAttribute('src') === 'https://example.com/chart.png' && content.querySelectorAll('.reader-remote-image').length === 1, 'The button loads only that image');
    window.__emit({type:'notice', text:'Native notice'});
    expect(document.getElementById('toast-msg').textContent === 'Native notice', 'Host notices appear as toasts');

    // The reading place saved for a file (host message) is restored when the file is shown again
    const placeDoc = Array.from({length: 120}, (_, i) => 'Place ' + i).join('\n\n');
    window.__emit({type: 'documentState', kind: 'readingPosition', bufferId: '51', value: '80:0'});
    await render(placeDoc, '51', -1);
    const placed = content.querySelector('[data-line="80"]').getBoundingClientRect().top;
    expect(Math.abs(placed) < 3, 'A saved reading place is restored for the file (' + Math.round(placed) + ')');
    window.__messages.length = 0;
    scrollTo(scrollRoot().scrollTop + 400);
    await new Promise(resolve => setTimeout(resolve, 800));
    expect(window.__messages.some(message => /^readingPosition:51:\d+:-?\d+$/.test(message)), 'Scrolling saves the new reading place for the file');

    // Scrolling: another document jumps to its caret without animation; typing on the caret line does not re-centre
    const longDoc = Array.from({length: 120}, (_, i) => 'Line ' + i).join('\n\n');
    await render(longDoc, '41', -1);
    scrollTo(scrollRoot().scrollHeight);
    await render(longDoc, '42', 0);
    expect(scrollRoot().scrollTop === 0, 'Switching to another document lands on its caret at once (no scroll animation from the old offset)');
    await render(longDoc, '42', 200);
    const centred = scrollRoot().scrollTop;
    await new Promise(resolve => setTimeout(resolve, 700));
    const afterMove = scrollRoot().scrollTop;
    scrollTo(afterMove - 120);
    await render(longDoc + '\n\nTyped', '42', 200);
    expect(afterMove > centred - 1 && Math.abs(scrollRoot().scrollTop - (afterMove - 120)) < 30, 'A render with an unchanged caret line keeps the reader where it is (no re-centring)');

    settings();
    const darkRow = row('Dark mode:');
    const beforeDark = window.__messages.length;
    expect(darkRow.getAttribute('aria-disabled') === 'true', 'DarkModeApplicability locks named palettes');
    darkRow.click(); setDarkModeFromMenu(0);
    expect(window.__messages.length === beforeDark && window._themeId === 'bearded-v-gold-d-raynh', 'DarkModeFixtureMatchesHost rejects named palette override');
    selectTheme('follow'); settings(); row('Dark mode:').click(); row('Light').click();
    expect(!document.body.classList.contains('dark'), 'DarkModeApplicability actually changes Follow palette');
    selectTheme('bearded-v-gold-d-raynh');

    settings(); row('Typography').click(); row('Text size:').click(); row('19px').click();
    openThemeView('typography'); row('Line height:').click(); row('190%').click();
    openThemeView('typography'); row('Reading width:').click(); row('680px').click();
    await settled();
    expect(getComputedStyle(document.getElementById('content')).fontSize === '19px', 'TypographyContentOnly font size');
    expect(Math.abs(parseFloat(getComputedStyle(document.getElementById('content')).lineHeight) - 36.1) < 0.1, 'TypographyContentOnly line height');
    expect(getComputedStyle(document.getElementById('content')).maxWidth === '680px', 'TypographyContentOnly reading width');
    const gearStyle = getComputedStyle(document.getElementById('theme-fab'));
    expect(Math.abs(parseFloat(gearStyle.width) - 34) < 0.1 && Math.abs(parseFloat(gearStyle.height) - 34) < 0.1 && getComputedStyle(row('680px')).fontSize === '13px', 'TypographyContentOnly preserves controls');
    const beforeTypography = window.__messages.length;
    setTypographyFromMenu('fontSize', 999);
    expect(window.__messages.length === beforeTypography, 'Typography rejects unbounded CSS values');
    setTypographyFromMenu('readingWidth', 0);
    await settled();
    const pane = document.getElementById('reader-wrap'), paneInner = pane.clientWidth - parseFloat(getComputedStyle(pane).paddingLeft) - parseFloat(getComputedStyle(pane).paddingRight);
    expect(Math.abs(document.getElementById('content').getBoundingClientRect().width - paneInner) < 2 && document.getElementById('theme-pop-list').textContent.includes('Whole pane'), 'Reading width "Whole pane" uses the full panel');
    // Top-level Reading width entry opens directly from Settings and returns there via Back
    settings();
    expect(row('Reading width:').textContent.includes('Whole pane'), 'Top-level Settings exposes Reading width');
    row('Reading width:').click(); finishAnimations();
    expect(document.querySelector('#theme-pop-list .theme-pop-item:nth-child(2)').textContent.includes('Whole pane'), 'Whole pane is the first Reading width choice');
    document.querySelector('#theme-pop-list [data-theme="__back"]').click(); finishAnimations();
    expect(document.querySelector('#theme-popover .theme-pop-title').textContent === 'Settings', 'Back from top-level Reading width returns to Settings');
    for (const [kind, value] of [['fontSize',15],['lineHeight',170],['readingWidth',860]]) setTypographyFromMenu(kind,value);

    // Nested task indents, prose table breakout, code-in-table balancing, and nested table containment
    await render(
        'Paragraph anchor\n\n' +
        '- [ ] Task level 1\n  - [x] Task level 2\n    - [ ] Task level 3\n\n' +
        '| No | File | Issue | Priority | Status | Detailed resolution description |\n' +
        '|---:|---|---|---|---|---|\n' +
        '| 1 | WebView2Viewer | Tab does not close on Ctrl+W | High | Open | Intercept accelerator keys in WebView2 and forward close command to Notepad++ cleanly |\n\n' +
        '| ID | Name | File path | Problem description |\n' +
        '|---|---|---|---|\n' +
        '| 1 | Parser | `src/NppMarkdownNext/VeryLongModuleName/SubmoduleName/ParserImplementation.cpp` | Longer multi-word description that must not be crushed to one narrow word column |\n\n' +
        '- List item:\n\n  | Key | Value | Comment |\n  |---|---|---|\n  | a | 1 | Nested table inside a list must never overflow the viewport |\n',
        '39'
    );
    const contentLeft39 = document.getElementById('content').getBoundingClientRect().left;
    const tasks39 = [...document.querySelectorAll('#content li.task-list-item')].map(el => el.getBoundingClientRect().left);
    expect(Math.abs(tasks39[0] - contentLeft39) < 2 && (tasks39[1] - tasks39[0]) >= 18 && (tasks39[2] - tasks39[1]) >= 18, 'Task lists align level 1 with content and indent each nested level cleanly');
    const tables39 = [...document.querySelectorAll('#content .reader-table-wrap')];
    const wideTable860 = tables39[0].querySelector('table').getBoundingClientRect();
    const content860 = document.getElementById('content').getBoundingClientRect();
    expect(Math.abs(wideTable860.left - content860.left) < 2 && wideTable860.width <= content860.width + 1, 'Wide prose table aligns with the 860px reading width instead of breaking out');
    const codeTableCols = [...tables39[1].querySelector('table').rows[0].cells].map(c => c.getBoundingClientRect().width);
    expect(codeTableCols[3] >= 200, 'Long inline code in a table cell does not crush neighboring prose columns');
    expect(tables39[2].getBoundingClientRect().right <= content860.right + 1, 'Nested table inside a list stays within the reading column');
    setTypographyFromMenu('readingWidth', 0);
    await settled();
    const wideTableFull = tables39[0].querySelector('table').getBoundingClientRect();
    expect(wideTableFull.width > 950 && wideTableFull.width <= document.getElementById('content').getBoundingClientRect().width + 1, 'Tables expand together with text when Reading width switches to Whole pane');
    setTypographyFromMenu('readingWidth', 680);
    await settled();
    const wideTable680 = tables39[0].querySelector('table').getBoundingClientRect();
    expect(wideTable680.width <= 681 && Math.abs(wideTable680.left - document.getElementById('content').getBoundingClientRect().left) < 2, 'Tables shrink together with text when Reading width switches to 680px');
    setTypographyFromMenu('readingWidth', 860);
    await settled();

    await render('# Same\n\n# Same\n\n# Привет Мир!\n\n# !!!\n');
    expect(window.readerDocumentIndex.headings.map(item => item.id).join(',') === 'same,same-1,привет-мир,section', 'StableHeadingAnchors unicode and duplicates');
    expect(!!document.getElementById('reader-heading-0'), 'StableHeadingAnchors retain a legacy alias');
    const imageHeading = '# ![Logo](data:image/gif;base64,R0lGODlhAQABAIAAAAAAAP///ywAAAAAAQABAAACAUwAOw==)\n';
    await render(imageHeading);
    expect(document.querySelector('#content h1').id === 'logo', 'ImageHeadingAlt produces the same semantic ID as native export');
    expect(document.querySelector('#toc-drawer .toc-item').textContent === 'Logo', 'ImageHeadingAlt supplies the outline label');
    expect(document.querySelector('#content h1 img').getAttribute('alt') === 'Logo', 'ImageHeadingAlt preserves the rendered image');
    await render('<h2 id="reader-heading-0">Manual</h2>\n\n# Content\n\n# Same\n');
    expect(document.getElementById('reader-heading-0').textContent === 'Manual', 'LegacyAnchorCollision preserves author ID');
    expect(document.querySelectorAll('[id="content"]').length === 1 && !!document.getElementById('content-1'), 'AuthorAndReservedIds protects reader controls');
    expect(document.querySelectorAll('[id="reader-heading-0"]').length === 1, 'LegacyAnchorCollision creates no duplicate alias');
    await render('# Intro\n\n# Reader Heading 0\n');
    expect(document.querySelectorAll('#content h1')[1].id === 'reader-heading-0', 'CanonicalHeadingBeforeLegacyAlias preserves the native HTML slug');
    expect(document.getElementById('reader-heading-0').tagName === 'H1', 'CanonicalHeadingBeforeLegacyAlias gives a real heading priority');
    await render('[Jump to section](#destination)\n\n' + ('Ordinary prose #same and https://example.com/#same\n\n').repeat(30) + '# Destination\n\n' + ('Tail text\n\n').repeat(30));
    document.querySelector('#content a[href="#destination"]').click(); await settled();
    expect(scrollRoot().scrollTop > 0 && document.getElementById('destination').getBoundingClientRect().top >= -1 && document.getElementById('destination').getBoundingClientRect().top < window.innerHeight, 'StableHeadingAnchors follows a real Markdown section link');
    expect(document.getElementById('content').textContent.includes('Ordinary prose #same and https://example.com/#same'), 'AnchorDoesNotRewriteProse preserves ordinary text and URLs');

    await render('# First\n\n# Second\n\n# Third\n');
    openToc(); closeThemePopover();
    const secondLink = document.querySelector('#toc-drawer [data-heading-id="second"]');
    secondLink.focus();
    await render('# First\n\n# Second\n\n# Third\n');
    expect(document.activeElement === secondLink && secondLink.isConnected, 'OutlineKeepsFocus reuses focused node');
    await render('# First\n\n# Third\n');
    expect(document.activeElement.isConnected && document.activeElement.closest('#toc-drawer'), 'OutlineKeepsFocus handles a removed heading');
    const longSection = '# First\n\n' + ('Long section words '.repeat(60) + '\n\n').repeat(40) + '# Second\n\n' + ('Tail words '.repeat(60) + '\n\n').repeat(12);
    await render(longSection);
    expect(scrollRoot() === document.body && document.body.scrollHeight > document.body.clientHeight, 'ReaderScrollRoot uses the actual Kelk body container');
    scrollTo(Math.max(100, document.getElementById('second').offsetTop / 2)); await settled();
    expect(document.querySelector('#toc-drawer [aria-current="location"]').dataset.headingId === 'first', 'OutlineCurrentAndEmpty identifies a long section between headings');
    scrollTo(scrollRoot().scrollTop + document.getElementById('second').getBoundingClientRect().top - 39); await settled();
    expect(document.querySelector('#toc-drawer [aria-current="location"]').dataset.headingId === 'second', 'OutlineCurrentAndEmpty uses the forty-pixel boundary');
    const bodyStyle = document.body.getAttribute('style'), htmlStyle = document.documentElement.getAttribute('style');
    try {
        scrollTo(0);
        document.body.style.height = 'auto'; document.body.style.overflow = 'visible';
        document.documentElement.style.height = 'auto'; document.documentElement.style.overflow = 'auto';
        await settled();
        expect(scrollRoot() === document.scrollingElement && scrollRoot().scrollHeight > scrollRoot().clientHeight, 'ReaderScrollRoot falls back to the document container');
        scrollTo(scrollRoot().scrollTop + document.getElementById('second').getBoundingClientRect().top - 39); await settled();
        expect(document.querySelector('#toc-drawer [aria-current="location"]').dataset.headingId === 'second', 'OutlineCurrentAndEmpty follows document-root scrolling too');
    } finally {
        scrollTo(0);
        if (bodyStyle === null) document.body.removeAttribute('style'); else document.body.setAttribute('style', bodyStyle);
        if (htmlStyle === null) document.documentElement.removeAttribute('style'); else document.documentElement.setAttribute('style', htmlStyle);
        await settled(); scrollTo(0);
    }
    closeToc();

    await render('<div data-source-unmapped="true"><p data-line="0">Unknown <input class="task-checkbox" type="checkbox" data-line="0"></p></div>');
    const beforeUnknown = window.__messages.length;
    document.querySelector('#content p').dispatchEvent(new MouseEvent('dblclick',{bubbles:true}));
    document.querySelector('#content input').click();
    expect(window.__messages.length === beforeUnknown, 'UnknownChildOfMappedList cannot inherit an interactive ancestor');
    await render('> - [x] literal [ ]\n>   - [ ] Nested\n\n<input type="checkbox" class="task-checkbox" data-line="0" data-task-key="forged">\n', '915');
    const checkboxes = [...document.querySelectorAll('#content input.task-checkbox')];
    expect(checkboxes.length === 3 && checkboxes[2].disabled, 'TaskOwnership rejects a raw HTML checkbox');
    const revision = nativeRevision;
    const taskMessages = () => window.__messages.filter(message => message.startsWith('toggleCheckbox:'));
    const beforeTasks = taskMessages().length;
    checkboxes[2].click();
    expect(taskMessages().length === beforeTasks, 'TaskOwnership forged HTML cannot send edits');
    checkboxes[0].setAttribute('data-line','900');
    checkboxes[0].click();
    expect(taskMessages().at(-1) === 'toggleCheckbox:915:' + revision + ':0:4:1', 'TaskSnapshot sends the original first marker with buffer and revision');
    expect(checkboxes[0].disabled, 'TaskSnapshot waits for the next native render before another edit');
    checkboxes[1].click();
    expect(taskMessages().at(-1) === 'toggleCheckbox:915:' + revision + ':1:6:0', 'TaskSnapshot preserves a nested quote marker column');
    for (const item of window.READER_CALLOUTS) {
        await render('> [!' + item.aliases[0] + ']\n> Body\n');
        expect(!!document.querySelector('#content .alert-' + item.type), 'CalloutPaletteRoles ' + item.type);
    }
    await render('> [!NOTE]\n> Body\n');
    expect(getComputedStyle(document.querySelector('#content .alert-note')).backgroundColor === 'rgb(22, 32, 59)', 'CalloutPaletteRoles fixed Gold note background');
    expect(color('#content .alert-title') === 'rgb(227, 144, 0)', 'CalloutPaletteRoles fixed Gold note bar/title');

    settings();
    document.getElementById('toast-msg').textContent = '';
    row('Copy HTML').click();
    const pendingCopy = window.__messages.findLast(message => message.startsWith('copyHtml:'));
    expect(!document.getElementById('toast-msg').textContent.includes('copied'), 'CopyAcknowledgment waits for native success');
    window.__emit({type:'operationResult',operation:'copyHtml',requestId:'old-viewer:1',status:'succeeded'});
    settings(); expect(row('Copy HTML').getAttribute('aria-disabled') === 'true', 'CopyAcknowledgment ignores stale result');
    window.__emit({type:'operationResult',operation:'copyHtml',requestId:'',status:'failed',detail:'Unrelated native request'});
    expect(row('Copy HTML').getAttribute('aria-disabled') === 'true', 'CopyAcknowledgment ignores empty native ID while pending');
    resultFor(pendingCopy,'failed','Clipboard busy');
    expect(document.getElementById('reader-operation-warning').textContent.includes('Clipboard busy'), 'CopyAcknowledgment shows a real failure');
    settings(); row('Copy HTML').click();
    resultFor(window.__messages.findLast(message => message.startsWith('copyHtml:')),'succeeded');
    expect(document.getElementById('reader-operation-warning').hidden && document.getElementById('toast-msg').textContent.includes('copied'), 'CopyAcknowledgment reports matching success');
    window.__emit({type:'operationResult',operation:'saveSettings',requestId:'',status:'failed',detail:'Settings directory is read-only'});
    expect(!document.getElementById('reader-settings-warning').hidden, 'SettingsSaveFailure stays visible');
    window.__emit(window.__state);
    expect(!document.getElementById('reader-settings-warning').hidden, 'SettingsSaveFailure survives a state update');
    window.__emit({type:'operationResult',operation:'saveSettings',requestId:'',status:'succeeded'});
    expect(document.getElementById('reader-settings-warning').hidden, 'SettingsSaveFailure clears on recovery');

    window.__state.sync = false; window.__emit(window.__state);
    const longDocument = '# Reading\n\n' + Array.from({length:70},(_,i) => 'Paragraph ' + i + ' ' + 'words '.repeat(12)).join('\n\n') + '\n';
    await render(longDocument,'501');
    const readingAnchor = () => document.querySelector('#content [data-line="40"]');
    scrollTo(scrollRoot().scrollTop + readingAnchor().getBoundingClientRect().top + 12); await settled();
    await render(longDocument,'502');
    expect(scrollRoot().scrollTop === 0, 'ReadingPositionPerBuffer starts an unvisited buffer at its beginning');
    await render(longDocument,'501');
    expect(Math.abs(readingAnchor().getBoundingClientRect().top + 12) <= 2, 'ReadingPositionPerBuffer restores the semantic anchor');
    window.__emit({type:'bufferClosed',bufferId:'501'});
    await render('# B\n\nShort\n','502'); scrollTo(0);
    await render(longDocument,'501');
    expect(readingAnchor().getBoundingClientRect().top > 40, 'ReadingPositionBoundaries does not resurrect a closed buffer');
    scrollTo(scrollRoot().scrollTop + readingAnchor().getBoundingClientRect().top + 12); await settled();
    for (let i=0;i<101;i++) await render('# Cache\n\nEntry ' + i + '\n', String(1000+i));
    scrollTo(0); await render(longDocument,'501');
    expect(readingAnchor().getBoundingClientRect().top > 40, 'ReadingPositionBoundaries evicts the oldest entry after one hundred buffers');
    window.__state.sync = true; window.__emit(window.__state);

    closeToc(); closeThemePopover(); scrollTo(0);
    await render(window.__initialRender.markdown);
    const fixtureHtml = '<!doctype html>' + document.documentElement.outerHTML;
    const frame = document.createElement('iframe');
    frame.style.cssText = 'position:fixed;left:0;top:0;z-index:100000;border:0;height:720px;width:320px';
    try {
        const loaded = new Promise(resolve => { frame.onload = resolve; });
        frame.srcdoc = fixtureHtml; document.body.appendChild(frame); await loaded;
        const child = frame.contentWindow;
        for (const width of [120,320,650,651,1200]) {
            for (const zoom of [1,2]) {
                frame.style.width = (width / zoom) + 'px'; frame.style.height = (720 / zoom) + 'px';
                child.openToc(); child.openSettingsPopover();
                await new Promise(resolve => child.requestAnimationFrame(() => child.requestAnimationFrame(resolve)));
                child.document.getAnimations().forEach(animation => animation.finish());
                await new Promise(resolve => child.requestAnimationFrame(resolve));
                child.positionThemePopover();
                const transition = parseFloat(child.getComputedStyle(child.document.getElementById('toc-drawer')).transitionDuration);
                const reducedMotion = child.matchMedia('(prefers-reduced-motion: reduce)').matches;
                expect(!!child.document.querySelector('#theme-fab svg') && (reducedMotion ? transition === 0 : transition > 0), 'NarrowSettingsBounds preserves the SVG and the motion preference');
                for (const selector of ['#theme-fab','#theme-popover','.toc-close']) {
                    const bounds = child.document.querySelector(selector).getBoundingClientRect();
                    expect(bounds.left >= -1 && bounds.right <= child.innerWidth + 1 && bounds.top >= -1 && bounds.bottom <= child.innerHeight + 1, 'NarrowSettingsBounds ' + width + 'px at ' + (zoom*100) + '% ' + selector);
                }
            }
        }
    } finally { frame.remove(); }
    // Language: the setting switches the reader's own text (static, menu, re-rendered controls); never the document.
    const ru = window.READER_STRINGS.ru, en = window.READER_STRINGS.en;
    window.__emit({type:'render',markdown:'# Заголовок\n\n```js\nx\n```\n\n| a | b |\n|---|---|\n| 1 | 2 |\n',bufferId:'31',caretLine:-1,renderRevision:'lang1',title:'Lang',isDark:true,isBiDi:true,themeId:window._themeId});
    await new Promise(requestAnimationFrame); finishAnimations();
    window.__emit(Object.assign({}, window.__lastRender, {lang: 'ru'})); await new Promise(requestAnimationFrame);
    expect(document.getElementById('content').lang === 'ru' && document.documentElement.lang === 'en', 'Content carries the document language, the page the interface language');
    settings();
    expect(row(en['set.language'].replace('{0}', en['set.languageAuto'])), 'Language row shows the Auto setting');
    row(en['set.language'].split('{0}')[0]).click(); finishAnimations();
    expect(document.querySelector('#theme-popover .theme-pop-title').textContent === en['set.languageTitle'], 'Language submenu has its title');
    expect(['As Notepad++', 'English', 'Русский'].every(label => row(label)), 'Language submenu offers Auto, English and Russian');
    row('Русский').click();
    await new Promise(resolve => setTimeout(resolve, 30)); await new Promise(requestAnimationFrame); finishAnimations();
    expect(window.__messages.includes('readerLanguage:ru'), 'Choosing Russian tells the host');
    expect(document.documentElement.dataset.uiLanguage === 'ru', 'The host switches the reader to Russian');
    expect(document.getElementById('toc-fab').title === ru['ui.outlineButton'] && document.getElementById('editor-fab').title === ru['ui.hideEditorButton'] && document.querySelector('.toc-title').textContent === ru['ui.outlineTitle']
        && document.getElementById('theme-fab').getAttribute('aria-label') === ru['ui.settingsButton'], 'Fixed controls switch to Russian');
    expect(document.getElementById('toc-filter').placeholder === ru['ui.filterHeadings'] && document.getElementById('toc-resize').title === ru['ui.resizeOutline'], 'Outline controls switch to Russian');
    expect(document.querySelector('#content .reader-copy-code').title === ru['ui.copyCode'], 'Re-rendered code buttons are in Russian');
    expect(document.querySelector('#content .reader-col-grip').getAttribute('aria-label') === ru['ui.resizeColumnLabel'].replace('{0}', '1'), 'Column grips are in Russian');
    expect(document.querySelector('#content h1').textContent.includes('Заголовок') && document.querySelector('#content code').textContent.trim() === 'x', 'The document text itself is unchanged');
    settings();
    expect(document.querySelector('#theme-popover .theme-pop-title').textContent === ru['set.title'] && row(ru['set.language'].replace('{0}', 'Русский')) && row(ru['set.sync']), 'Settings menu is in Russian');
    row(ru['set.theme'].split('{0}')[0]).click(); finishAnimations();
    expect(row(ru['theme.follow']) && row(ru['theme.light']), 'Built-in theme names are translated');
    settings(); row(ru['set.language'].split('{0}')[0]).click(); finishAnimations();
    row(ru['set.languageAuto']).click();
    await new Promise(resolve => setTimeout(resolve, 30)); await new Promise(requestAnimationFrame); finishAnimations();
    expect(document.documentElement.dataset.uiLanguage === 'en' && document.getElementById('toc-fab').title === en['ui.outlineButton'], 'Auto returns to the host language');
    closeThemePopover();
    window.__state.sync = true; window.__emit(window.__state);
    selectTheme('bearded-v-gold-d-raynh');
    window.__emit({type: 'render', markdown: '', themeId: window._themeId, isDark: true, isBiDi: true});
    finishAnimations();
    expect(document.getElementById('content').textContent.includes('No content to preview'), 'An empty document clears the old preview');
    return {checks: checks.length, themes: options.length, settings: 13, gold, passed: true};
}
