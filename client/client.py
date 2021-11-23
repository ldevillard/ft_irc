import select
import tkinter as tk
import socket
import sys
import asyncio
from threading import Thread
from time import sleep, thread_time


class Server:
    _address: str
    _port: int
    _nickname: str
    _username: str
    _password: str

    def __init__(self, address, port, nickname, username, password):
        self._address = address
        self._port = int(port)
        self._nickname = nickname
        self._username = username
        self._password = password


def close(error=None, code=0):
    if (error != None):
        print(error)
    sys.exit(code)


def closeAppHandler(event=None):
    close()


class ConnectFrame:
    _root: tk.Tk
    _return: Server
    _topFrame: tk.Frame
    _botFrame: tk.Frame

    def __init__(self):
        self._root = tk.Tk()
        self._root.title("ft_irc : connect")
        self._root.maxsize(300, 200)
        self._root.minsize(300, 200)

        self._topFrame = tk.Frame(self._root)
        self._topFrame.pack(side="top", fill="both", expand=True)

        self._botFrame = tk.Frame(self._root)
        self._botFrame.pack(side="bottom", fill="both")

        self._topFrame.grid_anchor("center")

        tk.Label(self._topFrame, text="Address").grid(
            row=0, column=0, sticky="e")
        tk.Label(self._topFrame, text="Port").grid(row=1, column=0, sticky="e")
        tk.Label(self._topFrame, text="Nickname").grid(
            row=2, column=0, sticky="e")
        tk.Label(self._topFrame, text="Username").grid(
            row=3, column=0, sticky="e")
        tk.Label(self._topFrame, text="Password").grid(
            row=4, column=0, sticky="e")

        address = tk.Entry(self._topFrame)
        address.focus_set()
        port = tk.Entry(self._topFrame)
        nickname = tk.Entry(self._topFrame)
        username = tk.Entry(self._topFrame)
        password = tk.Entry(self._topFrame, show='*')

        address.grid(row=0, column=1)
        port.grid(row=1, column=1)
        nickname.grid(row=2, column=1)
        username.grid(row=3, column=1)
        password.grid(row=4, column=1)

        def fetchForm(event=None):
            self._return = Server(address.get(), port.get(
            ), nickname.get(), username.get(), password.get())
            self._root.unbind("<Destroy>")
            self._root.destroy()

        self._botFrame.grid_anchor("center")
        tk.Button(self._botFrame, text="Connect",
                  command=fetchForm).grid(row=0, column=0, pady=4)

        self._root.bind("<Return>", lambda e: fetchForm(e))
        self._root.bind("<Destroy>", lambda e: closeAppHandler(e))
        self._root.bind("<Escape>", lambda e: closeAppHandler(e))

    def loop(self):
        self._root.mainloop()


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
        self._text.insert(tk.END, text)
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
        chatpannel.addMessage(self._input.get()+"\n")
        self._input.delete(0, tk.END)


listServer = ("#General", "#memes", "#nsfwUwU")


class IRCClient:
    _socket: socket.socket
    _server: Server
    _channel: str

    def __init__(self, server: Server):

        self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._server = server

        self._socket.settimeout(0.5)

    def connect(self):
        self._socket.connect((self._server._address, self._server._port))

        self._channel = "#general"

    def get_res(self):
        try:
            return str(self._socket.recv(512))
        except(socket.timeout):
            return ""

    def send_cmd(self, cmd: str, msg: str):
        command = "{} {}\r\n".format(cmd, msg)
        self._socket.send(command.encode("UTF-8"))

    def join_channel(self):
        self.send_cmd("JOIN", self._channel)


class MainFrame:
    _root = tk.Tk
    _serverPannel: ServerPannel
    _chatPannel: ChatPannel
    _inputForm: InputForm
    _topFrame: tk.Frame
    _botFrame: tk.Frame
    _irc: IRCClient
    _server: Server
    _thread: Thread
    _continueThread: bool

    def __init__(self, server: Server):
        self._server = server
        self._root = tk.Tk()
        self._root.title("ft_irc : connected to " +
                         server._address+":"+str(server._port)+" as "+server._username)
        self._root.geometry("700x500")
        self._root.minsize(400, 300)

        self._topFrame = tk.Frame(self._root)
        self._topFrame.pack(side="top", fill="both", expand=True)
        self._botFrame = tk.Frame(self._root)
        self._botFrame.pack(side="bottom", fill="both")

        self._serverPannel = ServerPannel(self._topFrame)
        self._chatPannel = ChatPannel(self._topFrame)
        self._inputForm = InputForm(self._botFrame)

        def submitMessage(event=None):
            self._inputForm.submit(self._chatPannel)

        def closeWThread(e):
            self._continueThread = False
            self._thread.join()
            closeAppHandler(e)

        self._root.bind("<Return>", lambda e: submitMessage(e))
        self._root.bind("<Escape>", lambda e: closeWThread(e))
        self._root.bind("<Destroy>", lambda e: closeWThread(e))

        self._serverPannel.setList(listServer)

    async def init(self):
        self._chatPannel.addMessage("* Initialize connection\n")
        self._irc = IRCClient(self._server)
        self._chatPannel.addMessage(
            "* Connecting to " + self._server._address+":"+str(self._server._port)+'\n')
        self._irc.connect()

        self._irc.send_cmd("NICK", self._server._username)
        self._irc.send_cmd(
            "USER", "{} * * :{}".format(self._server._username, self._server._username))

    def update(self):
        while self._continueThread:
            data = self._irc.get_res()
            print(data)
            self._chatPannel.addMessage(data)
            sleep(1)

    def loop(self):
        self._root.mainloop()


def thread_process(mainFrame: MainFrame):
    asyncio.run(mainFrame.update())


async def run(mainFrame: MainFrame):
    await mainFrame.init()
    mainFrame._continueThread = True
    mainFrame._thread = Thread(target=mainFrame.update)
    mainFrame._thread.start()


if __name__ == '__main__':
    connectForm = ConnectFrame()
    connectForm.loop()
    mainFrame = MainFrame(connectForm._return)
    asyncio.run(run(mainFrame))
    mainFrame.loop()
