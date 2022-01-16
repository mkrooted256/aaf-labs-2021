import numpy as np
import pandas as pd

class Init:
    def __init__(self):
        self.init_roz=0
        self.buffer = []
    def tables(self,tabl):
        """F-друку"""
        colomses = tabl[0]
        ind = np.arange(1, len(tabl))
        tabl_for_print = pd.DataFrame(tabl[1:], columns=colomses, index=ind)
        print('************TABLES************')
        print(tabl_for_print)
        print('**************END*************')
    def parser(self,sob):
        """F-парсер"""
        sob = sob.replace('\n', ' ')
        sob = sob.replace('  ', ' ')
        sob = sob.replace('  ', ' ')
        sob = sob.replace('   ', ' ')
        sob = sob.replace(',', '')
        sob = sob.replace('(', ' ')
        sob = sob.replace(')', ' ')
        sob = sob.replace(';', ' ')
        sob = sob.replace('"', ' ')
        return sob
    def analiz(self,sob):
        """F-парсер-first"""
        sob=self.parser(sob)
        if sob[0:6] == 'CREATE':
            self.create(sob.split())
            print(sob.split())
        if sob[0:6] == 'INSERT':
            print(sob.split())
            self.add(sob.split())
        if sob[0:6] == 'SELECT':
            print(sob.split())
            self.get(sob.split())
        if sob[0:6] == 'DELETE':
            self.deleti(sob.split())
            print(sob.split())
            pass
        else:
            print('=/=')

    def add(self,sob):
        m=0
        k=[]
        try:
            for i in range(len(sob)-3):
                k.append(sob[3+i])
            for i in range(len(self.buffer)):
                if sob[2]==list(self.buffer[i].keys())[0]:
                    for j in range(len(k)):
                        self.buffer[i][sob[2]].append(k[j])
                    m=i
                    break
            #self.buffer[m][sob[2]]=np.array(self.buffer[m][sob[2]])
            print(self.buffer[m][sob[2]])
        except:
            print('Error')
    def deleti2(self,sob):
        try:
            m = 0
            for i in range(len(self.buffer)):
                if sob[1] == list(self.buffer[i].keys())[0]:
                    m = i
                    break
            print(self.buffer[m][sob[1]])
            testi = np.array(self.buffer[m][sob[1]])
            g = (testi.reshape(int(len(testi) / self.init_roz), self.init_roz))
            print(g)
            iniui=sob[3]
            prueer=999
            for i in range(len(g[0])):
                if(g[0][i]==iniui):
                    prueer=i
                    break
            if(prueer==999):
                print('Not Found')
                return 0
            else:
                for j in range(len(g)):
                    print(g[j][prueer])
                    if(g[j][prueer]==sob[5]):
                        g = np.delete(g, (j), axis=0)
                        break
                    else:
                        pass
            buff=[]
            for i in range(len(g)):
                for j in range(len(g[i])):
                    buff.append(g[i][j])
            self.buffer[m][sob[1]]=buff
        except:
            print('Error delete')
    def deleti(self,sob):
        n=True
        #try:
        m = 0
        for i in range(len(self.buffer)):
            if sob[1] == list(self.buffer[i].keys())[0]:
                m = i
                break
        print(self.buffer[m][sob[1]])
        testi = np.array(self.buffer[m][sob[1]])
        g = (testi.reshape(int(len(testi) / self.init_roz), self.init_roz))
        print(g)
        for i in range(len(g)):
            for j in range(len(g[i])):
                if g[i][j]==sob[5]:
                    self.deleti2(sob)
    def create(self,sob):
        numb = 2
        sp = []
        for j in range(int(len(sob) / 2 - 1)):
            sp.append(sob[numb+1])
            numb += 2
        try:
            self.buffer.append({sob[1]:sp})
            self.init_roz=int(len(sp))
        except:
            print('Error')
    def get(self,sob):
        print(f'{len(sob)}')
        if 'WHERE' in sob:
            if 'GROUP_BY' in sob:
                gr=sob[-1:]
                k = sob.index('WHERE')
                s = sob[k:-2]
                sob = sob[:k]
                k = []
                b = sob[5:]
                for j in range(len(b)):
                    k.append(b[j])
                print(k)
                m = 0
                try:
                    for i in range(len(self.buffer)):
                        if sob[3] == list(self.buffer[i].keys())[0]:
                            m = i
                            break
                    testi = np.array(self.buffer[m][sob[3]])
                    print(testi)
                    try:
                        g = testi.reshape(int(len(testi) / self.init_roz), self.init_roz)
                        # self.tables(g)
                        l = []
                        for i in range(len(g[0])):
                            if g[0][i] in k:
                                l.append(i)
                        k = []

                        for j in range(int(len(testi) / self.init_roz)):
                            for i in l:
                                k.append(g[j][i])

                        k = np.array(k)
                        k = k.reshape(int(len(k) / len(l)), len(l))
                        print(k)
                        g = []
                        g.append(k[0])
                        for i in range(1, len(k)):
                            if s[-1] in k[i]:
                                g.append(k[i])
                        g = np.array(g)
                        g=list(g)
                        ind=None
                        for i in range(len(g[0])):
                            if g[0][i]==gr[0]:
                                ind=i
                                break
                        s=g[0]
                        print(s)
                        g=g[1:]
                        n=ind
                        def sort_col(i):
                            return i[n]

                        g.sort(key=sort_col)
                        g.insert(0,s)
                        g = np.array(g)
                        self.tables(g)
                    except:
                        g = testi

                        l = []
                        for i in range(len(g[0])):
                            if g[0][i] in k:
                                l.append(i)
                        k = []

                        for j in range(int(len(testi) / self.init_roz)):
                            for i in l:
                                k.append(g[j][i])

                        k = np.array(k)
                        k = k.reshape(int(len(k) / len(l)), len(l))

                        g = []
                        g.append(k[0])
                        for i in range(1, len(k)):
                            if s[-1] in k[i]:
                                g.append(k[i])
                        g = np.array(g)
                        g = list(g)
                        ind = None
                        for i in range(len(g[0])):
                            if g[0][i] == gr[0]:
                                ind = i
                                break
                        s = g[0]
                        print(s)
                        g = g[1:]
                        n = ind

                        def sort_col(i):
                            return i[n]

                        g.sort(key=sort_col)
                        g.insert(0, s)
                        g = np.array(g)
                        self.tables(g)
                except:
                    print("Error")
            else:
                k=sob.index('WHERE')
                s=sob[k:]
                sob=sob[:k]
                k = []
                b = sob[5:]
                for j in range(len(b)):
                    k.append(b[j])
                print(k)
                m = 0
                try:
                    for i in range(len(self.buffer)):
                        if sob[3] == list(self.buffer[i].keys())[0]:
                            m = i
                            break
                    testi = np.array(self.buffer[m][sob[3]])
                    print(testi)
                    try:
                        g = testi.reshape(int(len(testi) / self.init_roz), self.init_roz)
                        l = []
                        for i in range(len(g[0])):
                            if g[0][i] in k:
                                l.append(i)
                        k = []

                        for j in range(int(len(testi) / self.init_roz)):
                            for i in l:
                                k.append(g[j][i])

                        k = np.array(k)
                        k = k.reshape(int(len(k) / len(l)), len(l))
                        print(k)
                        g=[]
                        g.append(k[0])
                        for i in range(1,len(k)):
                            if s[-1] in k[i]:
                                g.append(k[i])
                        g=np.array(g)
                        self.tables(g)
                    except:
                        g = testi

                        l = []
                        for i in range(len(g[0])):
                            if g[0][i] in k:
                                l.append(i)
                        k = []

                        for j in range(int(len(testi) / self.init_roz)):
                            for i in l:
                                k.append(g[j][i])

                        k = np.array(k)
                        k = k.reshape(int(len(k) / len(l)), len(l))

                        g = []
                        g.append(k[0])
                        for i in range(1, len(k)):
                            if s[-1] in k[i]:
                                g.append(k[i])
                        g = np.array(g)
                        self.tables(g)
                except:
                    print("Error")
        elif len(sob)==6:
            m = 0
            try:
                for i in range(len(self.buffer)):
                    if sob[4] == list(self.buffer[i].keys())[0]:
                        m = i
                        break
                testi = np.array(self.buffer[m][sob[4]])
                print(testi)
                try:
                    g=testi.reshape(int(len(testi) / self.init_roz), self.init_roz)
                    self.tables(g)
                except:
                    self.tables(testi)
            except:
              print("Error")
        else:
            k=[]
            b=sob[6:]
            for j in range(len(b)):
                k.append(b[j])
            print(k)
            m = 0
            try:
                for i in range(len(self.buffer)):
                    if sob[4] == list(self.buffer[i].keys())[0]:
                        m = i
                        break
                testi = np.array(self.buffer[m][sob[4]])
                print(testi)
                try:
                    g = testi.reshape(int(len(testi) / self.init_roz), self.init_roz)
                    #self.tables(g)
                    l=[]
                    for i in range(len(g[0])):
                        if g[0][i] in k:
                            l.append(i)
                    k=[]

                    for j in range(int(len(testi) / self.init_roz)):
                        for i in l:
                            k.append(g[j][i])

                    k=np.array(k)
                    k=k.reshape(int(len(k) / len(l)), len(l))

                    self.tables(k)
                except:
                    g=testi

                    l = []
                    for i in range(len(g[0])):
                        if g[0][i] in k:
                            l.append(i)
                    k = []

                    for j in range(int(len(testi) / self.init_roz)):
                        for i in l:
                            k.append(g[j][i])

                    k = np.array(k)
                    k = k.reshape(int(len(k) / len(l)), len(l))

                    self.tables(k)
            except:
                print("Error")