#!/usr/bin/env python
# -*- coding: utf-8 -*-

import re
import time
import tkinter
import concurrent.futures
from pymystem3 import Mystem
from tkinter import filedialog, messagebox
from collections import Counter 

def counter(words, num):
    occurences = Counter(words)
    sorted_occ = sorted(occurences.items(), key=lambda item: item[1], reverse=True)
    return sorted_occ[num][0], sorted_occ[num][1]

def n_most_frequent_words(n, window):
    words = window.text_box.get(1.0, tkinter.END)
    window.task_button["state"] = "disabled"
    mystem = Mystem()
    pattern = re.compile("[А-Яа-я]")
    lemmas = mystem.lemmatize(words)
    lemmas = [i for i in lemmas if (pattern.match(i))]
    results = []
    outputs = []
    then = time.time()
    with concurrent.futures.ThreadPoolExecutor() as executor:
        for i in range(n):
            results.append(executor.submit(counter, lemmas, i))
        outputs = [future.result() for future in concurrent.futures.as_completed(results)]
        outputs = sorted(outputs, key=lambda item: item[1], reverse=True)
    now = time.time()
    tkinter.messagebox.showinfo("Найдено топ " + str(n) + " из самых часто-встречающихся слов:",
                                str(outputs).strip('[]') + "\nПрошло " + str(float("{:.5f}".format(now-then))) + " сек.")
    window.task_button["state"] = "normal"
    

class TextWindow:
    def __init__(self, master, filename):
        self.master = master
        self.master.title(filename)
        self.f = open(filename, encoding='utf-8')
        self.openedWindow = tkinter.Toplevel(self.master) 
        self.scrollbar = tkinter.Scrollbar(self.openedWindow)
        self.text_box = tkinter.Text(self.openedWindow, width=100, height=25,
                                     padx=10, pady=10, borderwidth=2,
                                     relief=tkinter.GROOVE, cursor="arrow",
                                     yscrollcommand=self.scrollbar.set, wrap=tkinter.WORD)
        self.scrollbar.config(command = self.text_box.yview)
        self.text_box.insert(tkinter.INSERT, self.f.read())
        self.scrollbar.pack(side="right", fill="y", expand=False)
        self.text_box.pack(side="right", fill="both", expand=True)
        self.text_box.config(state=tkinter.DISABLED)
        self.task_label = tkinter.Label(self.openedWindow,
                                        text="Введите n:",
                                       font=("Calibri", 15))
        self.task_label.pack(padx = 10, pady = (70, 10))
        self.task_n = tkinter.Text(self.openedWindow, width=10, height=2,
                                   borderwidth=2, relief=tkinter.GROOVE)
        self.task_n.pack()
        self.task_button = tkinter.Button(self.openedWindow,
                                          text="Найти n самых частых слов",
                                          command=self._n_most_frequent_words)
        self.task_button.pack(padx = 10, pady = 10)


    def _n_most_frequent_words(self):
        try:
            n = int(self.task_n.get(1.0, tkinter.END))
            concurrent.futures.ThreadPoolExecutor().submit(n_most_frequent_words, n, self)
        except:
            tkinter.messagebox.showinfo("Ошибка", "Введите число")

class StartWindow:
    def __init__(self, master):
        self.master = master
        self.master.title("lab_parallel_6")
        self.header = tkinter.Label(self.master,
                                    text="Поиск n самых часто-встречающихся слов",
                                   font=("Calibri", 18))
        self.header.pack(padx = 10, pady = (70, 10))
        self.upload_button = tkinter.Button(self.master,
                                            text="Выберите файлы для анализа",
                                            command=self._upload_file)
        self.upload_button.pack(padx = 50, pady = 50)
        
    def _upload_file(self):
        files = tkinter.filedialog.askopenfilenames(title="Выберите один или несколько файлов.",
                                                filetypes=[("Text Files", "*.txt")])
        for f in files:
            TextWindow(self.master, f)
            

root = tkinter.Tk()
root.geometry('600x300+400+200')
mw = StartWindow(root)
root.mainloop()