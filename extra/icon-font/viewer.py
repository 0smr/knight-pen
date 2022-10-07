import os
from http.server import SimpleHTTPRequestHandler, HTTPServer

class handler(SimpleHTTPRequestHandler):
    def do_GET(self):

        if self.path == "/":
            self.send_response(200)
            self.send_header('Content-type','text/html')
            self.end_headers()
            content = createBody()
            self.wfile.write(bytes(content, "utf8"))
        elif os.path.isfile(self.path[1:]):
            self.send_response(200)
            self.send_header('Content-type','image/svg+xml')
            self.end_headers()
            self.wfile.write(bytes(open(self.path[1:], 'r').read(), "UTF-8"))
        else:
            self.send_response(404)
            self.send_header('Content-type','text/html')
            self.end_headers()
            self.wfile.write(bytes("404", "utf8"))

def createBody():
    file = open('viewer.html', 'r')
    data = file.read()
    file.close()

    dir = "SVG"
    glyphPathList = os.listdir(dir)
    htmlGrid = ""
    table = {}

    for glyphPath in glyphPathList:
        glyphFile = os.path.basename(glyphPath) # remove path
        rowKey = glyphFile[0:3] # row key is first 3 characters of file name (e.g. "e00")

        if rowKey not in table:
            table[rowKey] = [None] * 16

        idx = int(glyphFile[3:4], 16)
        table[rowKey][idx] = glyphFile

    for key in table:
        glyphRow = table[key]
        htmlGrid += '<div class="row">\n'
        for idx, glyph in enumerate(glyphRow):
            if glyph:
                htmlGrid += '\t\t<div class="col"><img src="{0}/{1}" alt="{2}"><i>{2}</i></div>\n' .format(dir, glyph, glyph[0:-4])
            else:
                htmlGrid += '\t\t<div class="col">{0}{1:x}</div>\n'.format(key, idx)
        htmlGrid += '</div>\n'

    data = data.replace('%unicode-view%', htmlGrid)
    return data

if __name__ == '__main__':
    server = HTTPServer(('', 8000), handler)
    print('Starting server, use <Ctrl-C> to stop (http://localhost:8000)')
    server.serve_forever()
