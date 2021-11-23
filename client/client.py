from os import stat
import tkinter as tk
import sys
from tkinter.constants import BOTH
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
        listVar = tk.StringVar(value=list)
        self._list = tk.Listbox(
            self._frame, listvariable=listVar, selectmode="extended")
        self._list.grid(row=0, column=0, sticky="NWES")
        self._scrollBar = tk.Scrollbar(
            self._frame, orient="vertical", command=self._list.yview)
        self._list.config(yscrollcommand=self._scrollBar.set)
        self._list.pack(side=tk.LEFT, fill="both", expand=True)
        self._scrollBar.pack(side=tk.RIGHT, fill="both")


class ChatPannel:
    _frame: tk.Frame
    _text: tk.Text
    _scrollBar: tk.Scrollbar

    def __init__(self, widget):
        self._frame = tk.Frame(widget)
        self._frame.pack(side=tk.RIGHT, fill="both", expand=True)

        self._text = tk.Text(self._frame, state="disabled", width=0)
        self._scrollBar = tk.Scrollbar(
            self._frame, orient="vertical", command=self._text.yview)
        self._text.config(yscrollcommand=self._scrollBar.set)
        self._text.pack(side=tk.LEFT, fill="both", expand=True)
        self._scrollBar.pack(side=tk.RIGHT, fill="both")

    def addMessage(self, text):
        self._text.config(state="normal")
        self._text.insert(tk.END, text+"\n")
        self._text.config(state="disabled")


list = ('Java', 'C#', 'C', 'C++', 'Python',
        'Go', 'JavaScript', 'PHP', 'Swift')


class mainFrame:
    _root = tk.Tk
    _serverPannel: ServerPannel
    _chatPannel: ChatPannel

    def __init__(self, server: Server):
        self._root = tk.Tk()
        self._root.title("ft_irc : connected to " +
                         server._address+":"+server._port)
        self._root.geometry("500x500")

        self._serverPannel = ServerPannel(self._root)
        self._chatPannel = ChatPannel(self._root)

        self._root.bind("<Escape>", lambda e: closeAppHandler(e))

        self._root.mainloop()


# chatPannel.addMessage("salut")
# chatPannel.addMessage("salut")
# chatPannel.addMessage("salut")
# chatPannel.addMessage(
#     "papeupipepupapeupipepupapeupipepupapeupipepupapeupipepupapeupipepupapeupipepupapeupipepupapeupipepupapeupipepu")
# chatPannel.addMessage("salut")
# chatPannel.addMessage("salut")
# chatPannel.addMessage("salut")
# chatPannel.addMessage("salut")
# chatPannel.addMessage("salut")
# chatPannel.addMessage("pouet")


if __name__ == '__main__':
    server: Server = connect()
    mainFrame(server)
