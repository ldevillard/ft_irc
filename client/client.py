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


class ChatPannel:
    _pannel: tk.Text
    _scrollBar: tk.Scrollbar

    def __init__(self, widget):
        self._pannel = tk.Text(widget, state="disabled", width=0)
        self._scrollBar = tk.Scrollbar(
            widget, orient="vertical", command=self._pannel.yview)
        self._pannel.config(yscrollcommand=self._scrollBar.set)

        self._pannel.pack(side=tk.LEFT, fill="both", expand=True)
        self._scrollBar.pack(side=tk.RIGHT, fill="both")

    def addMessage(self, text):
        self._pannel.config(state="normal")
        self._pannel.insert(tk.END, text+"\n")
        self._pannel.config(state="disabled")


def mainFrame(server: Server):
    root = tk.Tk()
    root.title("ft_irc : connected to "+server._address+":"+server._port)
    root.geometry("500x500")

    chatPannel = ChatPannel(root)

    chatPannel.addMessage("salut")
    chatPannel.addMessage("salut")
    chatPannel.addMessage("salut")
    chatPannel.addMessage("papeupipepupapeupipepupapeupipepupapeupipepupapeupipepupapeupipepupapeupipepupapeupipepupapeupipepupapeupipepu")
    chatPannel.addMessage("salut")
    chatPannel.addMessage("salut")
    chatPannel.addMessage("salut")
    chatPannel.addMessage("salut")
    chatPannel.addMessage("salut")
    chatPannel.addMessage("pouet")

    root.bind("<Escape>", lambda e: closeAppHandler(e))

    root.mainloop()


if __name__ == '__main__':
    server: Server = connect()
    mainFrame(server)
