import parser0
import re

import sys

from table import Table


def create(args, list):
    name = args[1][0].strip()
    created = 0
    for l in list:
        if l[1]==name:
            created=1
    if created==1:
        print("Name " + name + " is already in use!")
        return
    tbl = Table(args[2:])
    list.append([tbl,name])
    print(name + " was successfully created!")
    return


def insert(args, list):
    name = args[1][0].strip()
    created = 0
    index = 0
    i=0
    for l in list:
        if l[1] == name:
            created = 1
            index = i
        i+=1
    if created == 0:
        print("Name " + name + " was never used!")
        return
    list[index][0].insertT(args[2:])
    print("Data ")
    print(args[2:] )
    print(" was successfully inserted!")
    return


def delete(args, list):
    name = args[1][0].strip()
    created = 0
    index = 0
    i = 0
    for l in list:
        if l[1] == name:
            created = 1
            index = i
        i += 1
    if created == 0:
        print("Name " + name + " was never used!")
        return
    where = []
    for el in args:
        if el[1] != "command" and el[1] != "tablename":
            where.append(el)
    list[index][0].deleteT(where)
    if len(where)<=1:
        print ("Table "+name+" is empty now!")



def select(args, list):
    name1 = ""
    name2 = ""
    selectCols = []
    join = []
    where = []
    for el in args:
        if el[1]=="tablename1":
            name1 = el[0].strip()
        elif el[1]=="tablename2":
            name2 = el[0].strip()
        elif el[1]=="colname":
            if re.search("[&]",el[0]):
                selectCols.append("&")
            else:
                selectCols.append(el[0].strip())
        elif el[1]=="join":
            if re.search("[$]",el[0].strip()):
                join.append("$")
            else:
                join.append(el[0].strip())
        elif el[1]!="command":
            where.append(el)



    created = 0
    index = 0
    i = 0
    for l in list:
        if l[1] == name1:
            created = 1
            index = i
        i += 1
    if created == 0:
        print("Name " + name1 + " was never used!")
        return

    if name2!="":
        created = 0
        index2 = 0
        i = 0
        for l in list:
            if l[1] == name2:
                created = 1
                index2 = i
            i += 1
        if created == 0:
            print("Name " + name2 + " was never used!")
            return
        list[index][0].selectT2(list[index2][0], selectCols, join, where)
    else:
        list[index][0].selectT1(selectCols, where)


if __name__ == '__main__':
    db = []
    res = ""

    while True:
        print("")
        print("Enter:")
        line = res + sys.stdin.readline() + " "
        if "exit" in line:
            break
        line = re.sub('\n', ' ', line)
        lSpit = line.split(";");
        res = lSpit[len(lSpit)-1]
        for i in range(0, len(lSpit)-1):
            el = parser0.parse(lSpit[i])
            print("Result of command: " + lSpit[i])
            if el[0][0] == "create":
                create(el, db)
            elif el[0][0] == "insert":
                insert(el, db)
            elif el[0][0] == "delete":
                delete(el, db)
            elif el[0][0] == "select":
                select(el, db)
            else:
                print("Error: ", el[0][1])




