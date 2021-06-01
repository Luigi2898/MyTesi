import PySimpleGUI as sg
import os.path
from copy import copy


class article():
    def __init__(self, name, author, tag, abstract, filename):
        self.name = name
        self.author = author
        self.tag = tag
        self.abstract = abstract
        self.filename = filename
    def __str__(self):
        return self.name + " -.-.- " + str(self.author).replace(',', ' - ').replace('[', '').replace(']', '').replace('\'', '') + " -.-.- " + str(self.tag).replace(',', ' - ').replace('[', '').replace(']', '').replace('\'', '') + " -.-.- " + self.abstract.replace('\n', ' ').replace('r', '') + " -.-.- " + self.filename

class shelf():
    def __init__(self):
        self.library = []
    def readLibrary(self):
        if not(os.path.isfile('.library.shelf')):
            f = open('.library.shelf', 'w')
            f.close()
        else: 
            with open('.library.shelf', 'r') as lib:
                lines = lib.readlines()
                for aL in lines:
                    name, authors, tags, abstract, filename = aL.split('-.-.-')
                    author = authors.split(' - ')
                    tag = tags.split(' - ')
                    self.library.append(copy(article(name, author, tag, abstract, filename)))
    def addToLibrary(self, newArticle):
        if not(newArticle in self.library):
            self.library.append(copy(newArticle))
    def writeLibrary(self):
        with open('.library.shelf', 'w') as lib:
            for anArticle in self.library:
                lib.write(str(anArticle) + '\n')
    def search(self, name=None, author=None, tag=None, abstract=None, filename=None):
        if name != None:
            for anArticle in self.library:
                if name == anArticle.name:
                    print("found")
                elif name in anArticle.name:
                    print("Almost found")
                else:
                    print("no match")
        

def open_add_window(filename, theShelf):
    layout=[[sg.Text('Please enter article info (separate tags and authors by \'-\' character), if something is blank it is not a problem.')],
    [sg.Text('Title', size=(15, 1)), sg.InputText()],
    [sg.Text('Authors', size=(15, 1)), sg.InputText()],
    [sg.Text('Tags', size=(15, 1)), sg.InputText()],
    [sg.Text('Abstract', size=(15, 1)), sg.InputText()],
    [sg.Button("Add", key="-ADD-"), sg.Button("Cancel", key="-CANCEL-")]]
    window = sg.Window("", layout, modal=True)
    choice = None
    while True:
        event, values = window.read()
        if event == "-ADD-":
            if '-.-.-' in values[0] or '-.-.-' in values[1] or '-.-.-' in values[2] or '-.-.-' in values[3]:
                sg.popup("Error!!\nFields cannot contain \'-.-.-\'.", keep_on_top=True)
            else:
                newArticle = article(values[0], values[1].split('-'), values[2].split('-'), values[3], filename)
                theShelf.addToLibrary(newArticle)
        if event == "-CANCEL-" or event == sg.WIN_CLOSED:
            break
    window.close()

# First the window layout in 2 columns
def explorer(lib):

    file_list_column = [
        [
        sg.Text("Document Folder"),
        sg.In(size=(25, 1), enable_events=True, key="-FOLDER-"),
        sg.FolderBrowse(),
        ],
        [
        sg.Listbox(
            values=[], enable_events=True, size=(40, 20), key="-FILE LIST-"
        )
        ],
    ]

    action_column = [
        [sg.Button('Open', key="-OPEN-"),
        sg.Button('Add file', key="-ADD-")]
    ]

    # For now will only show the name of the file that was chosen

    # ----- Full layout -----
    layout = [
        [
        sg.Column(file_list_column),
        sg.VSeperator(),
        sg.Column(action_column, element_justification='center'),
        ]
    ]
    window = sg.Window("", layout)
    while True:
        event, values = window.read()
        if event == "Exit" or event == sg.WIN_CLOSED:
            break
        # Folder name was filled in, make a list of files in the folder
        if event == "-FOLDER-":
            folder = values["-FOLDER-"]
            try:
                # Get list of files in folder
                file_list = os.listdir(folder)
            except:
                file_list = []

            fnames = [
                f
                for f in file_list
                if os.path.isfile(os.path.join(folder, f))
                and f.lower().endswith((".pdf"))
            ]
            window["-FILE LIST-"].update(fnames)
        elif event == "-FILE LIST-":  # A file was chosen from the listbox
            try:
                filename = os.path.join(
                    values["-FOLDER-"], values["-FILE LIST-"][0]
                ).replace('/', '\\')
            except:
                pass
        elif event == '-OPEN-':
            try:
                os.system("explorer.exe " + filename + "&")
            except:
                sg.popup("Error!!\nPlease select a file to open.", keep_on_top=True)
        elif event == '-ADD-':
            try:
                os.system("explorer.exe " + filename + "&")
            except:
                sg.popup("Error!!\nPlease select a file to open.", keep_on_top=True)
            else:
                open_add_window(filename, lib)

    window.close()

def db(lib):
    layout=[[sg.Text('Please enter a query and select where to find.')],
    [sg.Text('Query', size=(15, 1)), sg.InputText()], 
    [sg.Button('Title', size=(15, 1), key='-TITLE-'), sg.Button('Authors', size=(15, 1), key='-AUTHOR-'), sg.Button('Tags', size=(15, 1), key='-TAG-'), sg.Button('Abstract', size=(15, 1), key='-ABSTRACT-')]]
    window = sg.Window("", layout, modal=True)
    while True:
        event, values = window.read()
        if event == "Exit" or event == sg.WIN_CLOSED:
            break
        if event == '-TITLE-':
            lib.search(name=values[0])
    window.close()




layout = [
    [sg.Button('File explorer', key="-FILE-"),
    sg.Button('Database explorer', key="-DB-")]
    ]
window = sg.Window("", layout)
lib = shelf()
while True:
    event, values = window.read()
    if event == "Exit" or event == sg.WIN_CLOSED:
        break
    # Folder name was filled in, make a list of files in the folder
    if event == "-FILE-":
        lib.readLibrary()
        explorer(lib)
        lib.writeLibrary()
    if event == '-DB-':
        lib.readLibrary()
        db(lib)

window.close()
