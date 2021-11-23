import tkinter as tk


class Server:
    _address: str
    _port: int
    _nickname: str
    _username: str
    _password: str

    def __init__(self):
        pass


def connect():
    master = tk.Tk()
    master.title("ft_irc : connect")

    tk.Label(master, text="Address").grid(row=0, column=0)
    tk.Label(master, text="Port").grid(row=1, column=0)
    tk.Label(master, text="Nickname").grid(row=2, column=0)
    tk.Label(master, text="Username").grid(row=3, column=0)
    tk.Label(master, text="Password").grid(row=4, column=0)

    address = tk.Entry(master)
    port = tk.Entry(master)
    nickname = tk.Entry(master)
    username = tk.Entry(master)
    password = tk.Entry(master, show='*')

    address.grid(row=0, column=1)
    port.grid(row=1, column=1)
    nickname.grid(row=2, column=1)
    username.grid(row=3, column=1)
    password.grid(row=4, column=1)

    formData: Server = Server()

    def fetchForm():
        formData._address = address.get()
        formData._port = port.get()
        formData._nickname = nickname.get()
        formData._username = username.get()
        formData._password = username.get()
        master.quit()

    tk.Button(master, text="Connect", command=fetchForm).grid(
        row=5, column=0, sticky=tk.W, pady=4)

    master.mainloop()

    return formData


def mainFrame(server: Server):
    master = tk.Tk()
    master.title("ft_irc : connected to "+server._address+":"+server._port)

    master.mainloop()


if __name__ == '__main__':
    server: Server = connect()
    mainFrame(server)
