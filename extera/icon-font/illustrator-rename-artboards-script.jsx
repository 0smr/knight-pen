var nameList = [
    ["square", "polygon", "triangle", "circle", "square-fill", "line", "knight-pen", "", "", "", "", "", "", "", "", ""],
    ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "", "", "", "", "", ""],
    ["eye-dropper", "brush", "pencil", "pen", "marker", "pen-wing", "pen-path", "stationery", "eraser", "canvas", "eye-drop", "", "", "", "", ""],
    ["orientation", "save", "", "views", "lock", "unlock", "", "clipboard", "clipboard-success", "clipboard-fail", "clipboard-paste", "", "", "", "", ""],
    ["move", "arrow-up", "arrow-right", "arrow-down", "arrow-left", "left-double",
        "right-double", "angle-left", "angle-right", "angle-up", "angle-down", "", "", "", "", ""],
    ["file", "stack", "", "", "history", "undo", "redo", "x", "plus", "", "", "", "", "", "", ""],
    ["eye", "eye-close", "", "", "direct-selection", "selection", "", "", "", "", "", "", "", "", "", ""],
    ["copyright", "at", "", "trash", "mouse-pointer", "", "", "", "", "", "", "", "", "", "", ""],
    ["", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""],
];

var col = nameList[0].length;

function renameAllArtboards() {
    var doc = app.activeDocument;
    var artLen = doc.artboards.length;

    for (var idx = 0; idx < artLen; idx++) {
        var i = Math.floor(idx / col);
        var j = idx % col;
        if (nameList[i][j] == "") {
            doc.artboards[idx].name = "z" + idx;
        } else {
            doc.artboards[idx].name = i.toString(16) + j.toString(16) + "-" + nameList[i][j];
        }
    }
}

function renameArtBoard_MAIN() {
    if (app.documents.length == 0) {
        alert("There is no active documents!");
    } else {
        renameAllArtboards();
    }
}

renameArtBoard_MAIN();