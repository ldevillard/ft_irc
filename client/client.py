import tkinter as tk


def connect():
    master = tk.Tk()
    master.title("ft_irc : connect")

    tk.Label(master, text="Address").grid(row=0, column=0)
    tk.Label(master, text="Port").grid(row=1, column=0)
    tk.Label(master, text="Nickname").grid(row=2, column=0)
    tk.Label(master, text="Username").grid(row=3, column=0)
    tk.Label(master, text="Password").grid(row=4, column=0)

    address = tk.Entry(master).grid(row=0, column=1)
    port = tk.Entry(master).grid(row=1, column=1)
    nickname = tk.Entry(master).grid(row=2, column=1)
    username = tk.Entry(master).grid(row=3, column=1)
    password = tk.Entry(master, show='*').grid(row=4, column=1)

    tk.Button(master, text="Connect", command=master.quit).grid(
        row=5, column=0, sticky=tk.W, pady=4)

    tk.mainloop()


if __name__ == '__main__':
    connect()
