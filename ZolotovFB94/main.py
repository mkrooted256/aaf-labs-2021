# variat 13
# потрібно встановити перед використанням pip install regex
import re

def parser(text): # парсер команд
    text = text[:text.index(";")].split()
    keyword = text[0].lower()
    args = text[1:]
    
    #print("First input = ",keyword, args)

    if keyword == "exit":
        return (keyword, '')

    if len(args) == 0:
        print("Not enough arguments")
        return -1

    if keyword == "create":
        # перевірка коректності команди
        if len(args) == 1:
            # не правильна назва:
            setname = args[0]
            if re.search("[a-zA-Z]", setname[0]) == None or re.search("[a-zA-Z0-9_]*", setname[1:])[0] != setname[1:]: # перевірка імені 
                print("Incorrect name")
                return -1
            else:
                return (keyword, setname)
        else:
            print("Wrong args")
            return -1

    elif keyword == "insert" or keyword == "contains":
        #print(args)

        if len(args) in [2,3]:
            setname = args[0]

            data = ' '.join([x for x in args[1:]]).replace(" ", "")
            if len(args) == 2: data = args[1]
            if re.search("^\[\d+,\d+\]*", data) != None: # перевірка [l, h]
                num1 = int(data[data.find("[")+1:data.find(",")])
                num2 = int(data[data.find(",")+1:data.find("]")])
                return (keyword, setname, num1, num2)
            else:
                print("Incorrect input, not like [l, h] l,h in N0") # вирішив числа мають бути Натуральні з нулем
                return -1
        else:
            print("Wrong args")
            return -1

    elif keyword == "print_tree":
        if len(args) == 1:
            setname = args[0]
            return (keyword, setname)
        else:
            print("Wrong args")
            return -1

    elif keyword == "search":

        setname = args[0]
        if len(args) == 1:
            return (keyword, setname, "")
        if args[1].lower() != 'where':
            print("incorrect keyword WHERE")
            return -1
        typewhere = args[2].lower()
        #print(args)
        if typewhere == 'contained_by' or typewhere == 'intersects':
            if len(args[3:]) in [1,2]:
                data = ' '.join([x for x in args[3:]]).replace(" ", "")
                if len(args) == 2: data = args[1]
                if re.search("^\[\d+,\d+\]*", data) != None: # перевірка [l, h]
                    num1 = int(data[data.find("[")+1:data.find(",")])
                    num2 = int(data[data.find(",")+1:data.find("]")])
                    return (keyword, setname, typewhere, num1, num2)
                else:
                    print("Incorrect input, not like [l, h] l,h in N0") # вирішив числа мають бути Натуральні з нулем
                    return -1
            else:
                print("Wrong args")
                return -1
        elif typewhere == 'right_of':
            if len(args[3:]) == 1:
                #print(args[3])
                if re.search("\D", args[3]) == None: # Перевірка х на число
                    num = int(args[3])                
                    return (keyword, setname, typewhere, num)
                else:
                    print("X is not number")
                    return -1
            else:
                print("Wrong args")
                return -1
        else:
            print("incorrect keyword after where")
            return -1
    else:
        print("Incorrect command")
        return -1
    
class Node(): # структура kd-дерева
    def __init__(self, num1, num2):
        self.data = (num1, num2)
        self.L = None
        self.R = None
     # level - порівняння по кординаті
    def insert(self, num1, num2, level = 0): # внесення елемента в дерево
        new_data = (num1, num2)
        if self.data == new_data:
            print("Already exists")
        elif new_data[level] < self.data[level]: # left
            if self.L is None:
                self.L = Node(num1, num2)
            else:
                self.L.insert(num1, num2, level^1)
        else: # right
            if self.R is None:
                self.R = Node(num1, num2)
            else:
                self.R.insert(num1, num2, level^1)

    def contains(self, num1, num2, level = 0): # перевірка елемента в дереві
        check_data = (num1, num2)

        if self.data == check_data:
            return True
        elif self.L != None and check_data[level] < self.data[level]:
            #print(num1, num2)
            return self.L.contains(num1, num2, level^1)
        elif self.R != None:
            #print(num1, num2)
            return self.R.contains(num1, num2, level^1)
        
        return False

    def print(self): # обхід дерева для search без where
        if self.L != None:
            self.L.print()
        print(f"[{self.data[0]}, {self.data[1]}]")
        if self.R != None:
            self.R.print()

    def get_children(self): # отримання елементів 
        result = []
        if self.L != None:
            result += self.L.get_children()
        if self.R != None:
            result += self.R.get_children()
        
        return result + [self.data]            

    def right_of(self, x, level = 0): # пошук справа
        result = []
        if level == 0: 
            if x <= self.data[0]: # елемент підходить то і його праве піддерево теж
                result += [self.data]
                if self.R != None:
                    result += self.R.get_children()
                if self.L != None:
                    result += self.L.right_of(x, level^1)
            else:
                if self.R != None:
                    result += self.R.right_of(x, level^1)

        else:
            if x <= self.data[1]:
                if self.L != None:
                    result += self.L.right_of(x, level^1)  
                if self.R != None:
                    result += self.R.right_of(x, level^1) 
                result += [self.data]
            else:
                return []
        return result    


    def contained_by(self, num1, num2, level = 0): # пошук тих що в межах 
        result = []

        if level == 0: 
            if num1 >= self.data[0]: 
                if self.R != None:
                    result += self.R.contained_by(num1, num2, level^1)
            else:
                if self.R != None:
                    result += self.R.contained_by(num1, num2, level^1)
                if self.L != None:
                    result += self.L.contained_by(num1, num2, level^1)
        else:
            if num2 <= self.data[1]:
                if self.L != None:
                    result += self.L.contained_by(num1, num2, level^1)                 
            else:
                if self.L != None:
                    result += self.L.contained_by(num1, num2, level^1)  
                if self.R != None:
                    result += self.R.contained_by(num1, num2, level^1)
        
            
        if num1 <= self.data[0] and num2 >= self.data[1]: # елемент підходить
                result += [self.data]
        return result    


            
    def intersects(self, num1, num2, level = 0): # пошук тих що пересікають
        result = []

        if level == 0: 
            if num1 >= self.data[0]: 
                if self.R != None:
                    result += self.R.intersects(num1, num2, level^1)
            else:
                if self.R != None:
                    result += self.R.intersects(num1, num2, level^1)
                if self.L != None:
                    result += self.L.intersects(num1, num2, level^1)
        else:
            if num2 <= self.data[1]:
                if self.L != None:
                    result += self.L.intersects(num1, num2, level^1)                 
            else:
                if self.L != None:
                    result += self.L.intersects(num1, num2, level^1)  
                if self.R != None:
                    result += self.R.intersects(num1, num2, level^1)
        
            
        if (num1 >= self.data[0] and num1 <= self.data[1]) or \
            (num2 >= self.data[0] and num2 <= self.data[1]) or \
            (num1 <= self.data[0] and num2 >= self.data[1]): # елемент підходить
                #print("good")
                result += [self.data]
        return result    



def print_tree(root, space = 0): # вивід дерева
 
    if root == None:
        return
    space += 7

    print_tree(root.R, space)
    print()

    for _ in range(7, space):
        print(end = " ")
    print(root.data)
 
    print_tree(root.L, space)

SETS = {} # місце зберігання множин

while True:
    text = input("Enter command: ")
    while text.find(';') == -1: # багаторядкове введення
        text += " "
        text += input(": ")

    command = parser(text)
    #print(SETS)
    if command == -1: continue
    #print("Command is ",command)

    if command[0] == "create":
        setname = command[1]
        if setname in SETS.keys():
            print(f"{setname} is already used")
            continue
        else:
            SETS[setname] = None
            print(f"Set {setname} has been created")
        #print(SETS)

    elif command[0] == "insert":
        setname, num1, num2 = command[1:]
        if setname in SETS.keys():
            if SETS[setname] == None:
                SETS[setname] = Node(num1, num2)
            else:
                SETS[setname].insert(num1, num2)
            print(f"Range [{num1}, {num2}] has been added to {setname}")
        else:
            print("setname not found")

    elif command[0] == "print_tree":
        setname = command[1]
        if setname in SETS.keys():
            print_tree(SETS[setname])
            #print(SETS[setname].get_children())
        else:
            print("setname not found")

    elif command[0] == "contains":
        setname = command[1]
        num1 = command[2]
        num2 = command[3]
        if setname in SETS.keys():
            print(SETS[setname].contains(num1, num2))
            
        else:
            print("setname not found")

    elif command[0] == "search":
        setname = command[1]
        typewhere = command[2]
        if setname in SETS.keys():
            if typewhere == '':
                SETS[setname].print()
            elif typewhere == 'contained_by':
                result = SETS[setname].contained_by(command[3],command[4])
                print(sorted(result, key=lambda tup: tup[0]))
            elif typewhere == 'intersects':
                result = SETS[setname].intersects(command[3],command[4])
                print(sorted(result, key=lambda tup: tup[0]))
            elif typewhere == 'right_of':
                result = SETS[setname].right_of(command[3])
                print(sorted(result, key=lambda tup: tup[0]))
        else:
            print("setname not found")
    elif command[0] == "exit":
        break