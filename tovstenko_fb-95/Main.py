import time
from Function import Init
if __name__ == '__main__':
    print('========================Start========================')
    sob=''
    cla=Init()
    while sob!='Exit':
        sob=input('DataBase>')
        cla.analiz(sob)