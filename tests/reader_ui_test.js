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
    expect(document.querySelectorAll('#theme-pop-list .theme-pop-item').length === 11, 'All ten legacy settings and Typography are present');
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
        window.__emit({type:'operationResult',operation,status,detail,error:status==='failed'?5:0,recoveryPath:'',requestId:command.slice(operation.length+1)});
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
    for (const [selector, expected] of [['h1','rgb(255, 208, 0)'],['strong','rgb(237, 242, 247)'],['em','rgb(255, 130, 63)'],[':not(pre) > code','rgb(161, 103, 255)'],['blockquote','rgb(228, 172, 115)'],['a','rgb(62, 178, 255)'],['.hljs-comment','rgb(51, 73, 132)']]) {
        expect(color('#content ' + selector) === expected, 'GoldMarkdownRoles ' + selector);
    }
    expect(color('#content') === 'rgb(184, 196, 228)', 'GoldMarkdownRoles body text');
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
    for (const [kind, value] of [['fontSize',15],['lineHeight',170],['readingWidth',860]]) setTypographyFromMenu(kind,value);

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
    window.__state.sync = true; window.__emit(window.__state);
    selectTheme('bearded-v-gold-d-raynh');
    window.__emit({type: 'render', markdown: '', themeId: window._themeId, isDark: true, isBiDi: true});
    finishAnimations();
    expect(document.getElementById('content').textContent.includes('No content to preview'), 'An empty document clears the old preview');
    return {checks: checks.length, themes: options.length, settings: 11, gold, passed: true};
}
