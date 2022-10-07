#!/usr/bin/env python3
import os
import glob
import shutil
import fontforge # sudo apt-get install python3-fontforge

def getAppVersion(): # get the most recent git tag as the version.
    try: return os.popen('git describe --tags --abbrev=0').read()[1:]
    except: return None

def separator(svgFile):
    splitted = os.path.basename(svgFile).split('.')[0].split('-')
    return { 'uni' : int(splitted[0], 16), 'name' : splitted[1] }

if __name__ == '__main__':
    sfdFont = fontforge.open("./knight-icon.sfd") # open sfd font file.

    # if git was available, set the font version.
    sfdFont.version = getAppVersion() or sfdFont.version 

    # ---------- start glyphs update ----------
    print('start updating glyphs {')

    svgFiles = glob.glob('Svg/*.svg')

    for file in svgFiles:
        fileInfo = separator(file) # extract data from the filename
        if(sfdFont.__contains__(fileInfo['uni']) == False): # if glyph doesn't exist.
            glyph = sfdFont.createChar(fileInfo['uni'], fileInfo['name']) # create glyph.
            glyph.importOutlines(file) # import svg outline.

            print('added new glyph - {0} ({1})'.format(fileInfo['name'], fileInfo['uni']))
        else:
            glyph = sfdFont.__getitem__(fileInfo['uni'])
            if(glyph.glyphname != fileInfo['name']): # update glyph info.
                glyph.clear() # clear glyph.
                glyph.importOutlines(file) # import svg outline.
                glyph.glyphname = fileInfo['name'] # set new name to glyph.
                print('updated glyph - {0} ({1})'.format(fileInfo['name'], fileInfo['uni']))

    print('} end updating glyphs') 
    # ---------- end glyphs update ----------

    sfdFont.save() # save sfd file.

    baseName = (sfdFont.default_base_filename or 'knight-icon-solid') + '.ttf'
    sfdFont.generate(baseName) # generate '.ttf' font.

    # copy the ttf file to the app resources folder and replace the old one.
    shutil.copy2(baseName, '../../src/knight-pen/resources/font/')

    # remove the generated font files.
    # os.remove(baseName)