from os import stat
import tkinter as tk
import sys
from tkinter.constants import ANCHOR, BOTH
from typing import Text


class Server:
    _address: str
    _port: int
    _nickname: str
    _username: str
    _password: str

    def __init__(self):
        pass


def close(error=None, code=0):
    if (error != None):
        print(error)
    sys.exit(code)


def closeAppHandler(event=None):
    close()


def connect():
    root = tk.Tk()
    root.title("ft_irc : connect")
    root.grid_anchor("center")
    tk.Label(root, text="Address").grid(row=0, column=0, sticky="e")
    tk.Label(root, text="Port").grid(row=1, column=0, sticky="e")
    tk.Label(root, text="Nickname").grid(row=2, column=0, sticky="e")
    tk.Label(root, text="Username").grid(row=3, column=0, sticky="e")
    tk.Label(root, text="Password").grid(row=4, column=0, sticky="e")

    address = tk.Entry(root)
    address.focus_set()
    port = tk.Entry(root)
    nickname = tk.Entry(root)
    username = tk.Entry(root)
    password = tk.Entry(root, show='*')

    address.grid(row=0, column=1)
    port.grid(row=1, column=1)
    nickname.grid(row=2, column=1)
    username.grid(row=3, column=1)
    password.grid(row=4, column=1)

    formData: Server = Server()

    def fetchForm(event=None):
        formData._address = address.get()
        formData._port = port.get()
        formData._nickname = nickname.get()
        formData._username = username.get()
        formData._password = username.get()
        root.unbind("<Destroy>")
        root.destroy()

    tk.Button(root, text="Connect", command=fetchForm).grid(
        row=5, column=0, sticky=tk.W, pady=4)

    root.bind("<Return>", lambda e: fetchForm(e))
    root.bind("<Destroy>", lambda e: closeAppHandler(e))
    root.bind("<Escape>", lambda e: closeAppHandler(e))

    root.mainloop()

    return formData


class ServerPannel:
    _frame: tk.Frame
    _list: tk.Listbox
    _scrollBar: tk.Scrollbar

    def __init__(self, widget):
        self._frame = tk.Frame(widget)
        self._frame.pack(side=tk.LEFT, fill="both")
        self._list = tk.Listbox(
            self._frame, selectmode="extended", height=0)
        self._list.grid(row=0, column=0, sticky="NWES")
        self._scrollBar = tk.Scrollbar(
            self._frame, orient="vertical", command=self._list.yview)
        self._list.config(yscrollcommand=self._scrollBar.set)
        self._list.pack(side=tk.LEFT, fill="both", expand=True)
        self._scrollBar.pack(side=tk.RIGHT, fill="both")

    def setList(self, list: tuple):
        self._list.config(listvariable=tk.StringVar(value=list))


class ChatPannel:
    _frame: tk.Frame
    _text: tk.Text
    _scrollBar: tk.Scrollbar

    def __init__(self, widget):
        self._frame = tk.Frame(widget)
        self._frame.pack(side=tk.RIGHT, fill="both", expand=True)

        self._text = tk.Text(self._frame, state="disabled", width=0, height=0)
        self._scrollBar = tk.Scrollbar(
            self._frame, orient="vertical", command=self._text.yview)
        self._text.config(yscrollcommand=self._scrollBar.set)
        self._text.pack(side=tk.LEFT, fill="both", expand=True)
        self._scrollBar.pack(side=tk.RIGHT, fill="both")

    def addMessage(self, text):
        isAtEnd = self._text.yview()[1] >= 0.9
        self._text.config(state="normal")
        self._text.insert(tk.END, text+"\n")
        self._text.config(state="disabled")
        if isAtEnd == True:
            self._text.yview_moveto(1.0)


class InputForm:
    _input: tk.Text
    _submit: tk.Button

    def __init__(self, widget):

        self._input = tk.Entry(widget, width=0)
        self._submit = tk.Button(widget, command=self.submit, text="Send")
        self._input.pack(side=tk.LEFT, fill="both", expand=True)
        self._submit.pack(side=tk.RIGHT, fill="both")

        self._input.focus_set()

    def submit(self, chatpannel: ChatPannel):
        chatpannel.addMessage(self._input.get())
        self._input.delete(0, tk.END)


listServer = ("#General", "#memes", "#nsfwUwU")


class mainFrame:
    _root = tk.Tk
    _serverPannel: ServerPannel
    _chatPannel: ChatPannel
    _inputForm: InputForm
    _topFrame: tk.Frame
    _botFrame: tk.Frame

    def __init__(self, server: Server):
        self._root = tk.Tk()
        self._root.title("ft_irc : connected to " +
                         server._address+":"+server._port+" as "+server._username)
        self._root.geometry("500x500")

        self._topFrame = tk.Frame(self._root)
        self._topFrame.pack(side="top", fill="both", expand=True)
        self._botFrame = tk.Frame(self._root)
        self._botFrame.pack(side="bottom", fill="both")

        self._serverPannel = ServerPannel(self._topFrame)
        self._chatPannel = ChatPannel(self._topFrame)
        self._inputForm = InputForm(self._botFrame)

        def submitMessage(event=None):
            self._inputForm.submit(self._chatPannel)

        self._root.bind("<Return>", lambda e: submitMessage(e))
        self._root.bind("<Escape>", lambda e: closeAppHandler(e))
        self._root.bind("<Destroy>", lambda e: closeAppHandler(e))

        # self._chatPannel.addMessage("salut")
        # self._chatPannel.addMessage("salut")
        # self._chatPannel.addMessage("salut")
        # self._chatPannel.addMessage(
        #     "papeupipepupapeupipepupapeupipepupapeupipepupapeupipepupapeupipepupapeupipepupapeupipepupapeupipepupapeupipepu")
        # self._chatPannel.addMessage("salut")
        # self._chatPannel.addMessage("salut")
        # self._chatPannel.addMessage("salut")
        # self._chatPannel.addMessage("salut")
        # self._chatPannel.addMessage("salut")
        # self._chatPannel.addMessage("pouet")

        self._serverPannel.setList(listServer)

        self._root.mainloop()


if __name__ == '__main__':
    server: Server = connect()
    mainFrame(server)
