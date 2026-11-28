"""Reader UI suite: builds the mock-host page (tests/reader_fixture.py) and runs tests/reader_ui_test.js in Edge.

Needs Python Playwright (pip install playwright) and Microsoft Edge; Edge is used as installed, so no browser
download is needed. Exit code 0 = every check passed.
"""
import subprocess
import sys
from pathlib import Path

root = Path(__file__).resolve().parents[1]
page = Path(subprocess.run([sys.executable, str(root / 'tests' / 'reader_fixture.py')], check=True,
                           capture_output=True, text=True).stdout.strip())
try:
    from playwright.sync_api import sync_playwright
except ImportError:
    sys.exit('Python Playwright is missing: pip install playwright')

with sync_playwright() as playwright:
    browser = playwright.chromium.launch(channel='msedge')
    try:
        tab = browser.new_page()
        errors = []
        tab.on('pageerror', lambda error: errors.append(str(error)))
        tab.goto(page.as_uri())
        tab.wait_for_function('window.__listeners && window.__listeners.length > 0 && !!document.getElementById("content")')
        tab.wait_for_timeout(300)  # the first render and settings arrive in microtasks after pageLoaded
        try:
            result = tab.evaluate((root / 'tests' / 'reader_ui_test.js').read_text(encoding='utf-8'))
        except Exception as error:  # a failed expect() throws with the check's description
            sys.exit('Reader UI suite failed: ' + str(error).splitlines()[0])
    finally:
        browser.close()

if errors or not result.get('passed'):
    sys.exit('Reader UI suite failed: ' + '; '.join(errors) if errors else 'Reader UI suite failed: ' + str(result))
print('Reader UI suite passed: %d checks, %d themes' % (result['checks'], result['themes']))
