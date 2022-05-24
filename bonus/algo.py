import os
import sys
from flask import Flask, render_template
from colorama import Fore, Back, Style

class prog():
    def __init__(self):
        self.passed = 0
        self.wrong = 0
        self.tab = []
        self.cat = []

class test():
    def __init__(self, test, name, cat):
        self.test_cmd = test
        self.cat = cat
        self.test_passed = 0
        self.answer_cmd = test
        self.test = ""
        self.answer = ""
        self.name = name
    def check_answer(self, prog):
        if self.test == self.answer:
            print(Fore.GREEN + self.name,":  passed")
            prog.passed += 1
            self.test_passed = 1
        else:
            self.test_passed = 0
            prog.wrong += 1
            print(Fore.RED + self.name,":  not passed")
            file_test_failed = open('./test_f/' + self.name, "w")
            file_test_failed.write("what we want :\n")
            file_test_failed.write(self.test)
            file_test_failed.write("\n\nwhat we have :\n")
            file_test_failed.write(self.answer)
            file_test_failed.close()

def site():
    app = Flask(__name__)
    @app.route('/')
    def index():
        return render_template('index.html', test = test_all.passed, all = test_all.passed + test_all.wrong, len = len(test_all.cat), cate = test_all.cat, lent = len(test_all.tab), tabb = test_all.tab, lenght = 0, passedf= 0)
    app.run(use_reloader = True, debug = True)
    pass

test_all = prog()
cat_act = ""
file_config = open("./test_f/config/test_conf", "r")
all_test = file_config.read()
all_test = all_test.split('\n')
b = ""
for i in range (len(all_test) - 1):
    if (all_test[i][0] == '&'):
        cat_act = ""
        for j in range (1, len(all_test[i])):
            cat_act = cat_act + all_test[i][j]
        test_all.cat.append(cat_act)
        pass
    else:
        b = ""
        c = ""
        a = all_test[i]
        a = a.split('=')
        for j in range (5, len(a[0]) - 1):
            b = b + a[0][j]
        for j in range (1, len(a[1]) - 1):
            c = c + a[1][j]
        test_all.tab.append(test(c, b, cat_act))

for j in range (0, len(test_all.tab)):
    print(test_all.tab[j].test_cmd)
    test_all.tab[j].answer_cmd = test_all.tab[j].test_cmd
    test_all.tab[j].test_cmd = test_all.tab[j].test_cmd + '> test'
    os.system(test_all.tab[j].test_cmd)
    test_all.tab[j].answer_cmd = 'echo "' + test_all.tab[j].answer_cmd + '" | 42sh > answer'
    os.system(test_all.tab[j].answer_cmd)
    file_test = open("test", "r")
    file_answer = open("answer", "r")
    test_all.tab[j].test = file_test.read()
    test_all.tab[j].answer = file_answer.read()
    file_answer.close()
    file_test.close()
    test_all.tab[j].check_answer(test_all)

print(Style.RESET_ALL)
print(test_all.wrong + test_all.passed, "tests", test_all.passed, "passed")
# site()