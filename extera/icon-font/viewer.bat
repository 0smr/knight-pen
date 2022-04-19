if not DEFINED IS_MINIMIZED set IS_MINIMIZED=1 && start "" /min "%~dpnx0" %* && exit
start chrome http://127.0.0.1:8000
cd "glyph viewer"
python viewer.py