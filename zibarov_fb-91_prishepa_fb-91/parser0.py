import re




def parse(command):
    res =[]
    command = re.sub("\\s+", " ", command)
    command = re.sub("\\(\\s*", " (", command)
    command = re.sub("\\s*\\)", ",)", command)
    command = re.sub("\\s*,\\s*", ",", command)
    command = command.strip()
    splt = command.split(" ")

    if re.match("(?i)create", splt[0]):
        res.append(["create", "command"])
        command = command.replace(splt[0]+" ", "", 1)
        if re.match("\w*", splt[1]):
            res.append([splt[1], "tblName"])
            command = command.replace(splt[1]+" ", "", 1).strip()
            if re.match("[(](?:\w+(\s+\w+)?[,])+[)]",command):
                command = command.replace("(", "")
                command = command.replace(")", "")
                args = command.split(",")
                del args[-1]
                for arg in args:
                    if re.match("\s*\w+\s+(?i)indexed\s*",arg):
                        res.append([arg.split(" ")[0], "1"])
                    elif re.match("\s*\w+\s*",arg):
                        res.append([arg, "0"])
                    else :
                        res.clear()
                        res.append(["5", "bad col name!"])
                        return res
            else:
                res.clear()
                res.append(["5", "bad col names!"])
                return res
        else:
            res.clear()
            res.append(["5", "bad table name!"])
            return res
    elif re.match("(?i)insert", splt[0]):
        res.append(["insert", "command"])
        command = command.replace(splt[0], "", 1).strip()
        if re.match("(?i)into", splt[1]):
            command = command.replace(splt[1], "", 1).strip()
        splt = command.split(" ")
        if re.match("\w*", splt[0]):
            res.append([splt[0], "tblName"])
            command = command.replace(splt[0], "", 1).strip()
            if re.match('[(](?:["]\s*\w+\s*["][,])+[)]', command):
                command = command.replace("(", "")
                command = command.replace(")", "")
                command = command.replace('"', "").strip()
                args = command.split(",")
                del args[-1]
                for arg in args:

                    if re.match("\s*\w+\s*", arg):
                        res.append(arg.strip())
                    else:
                        res.clear()
                        res.append(["5", "bad col name!"])
                        return res
            else:
                res.clear()
                res.append(["5", "bad col names!"])
                return res
        else:
            res.clear()
            res.append(["5", "bad table name!"])
            return res
    elif re.match("(?i)select", splt[0]):
        res.append(["select", "command"])
        command = command.replace(splt[0] + " ", "", 1).strip()
        if re.search("\s+(?i)from\s+", command):
            splitSearch = re.split("\s+(?i)from\s+",command)
            if re.match("\s*[*]\s*",splitSearch[0]):
                res.append(["&AllCols", "colname"])
            elif re.match("(?:\s*\w+\s*[,])+", splitSearch[0]+","):
                cols = splitSearch[0].split(",")
                for col in cols:
                   res.append([col, "colname"])
            else:
                res.clear()
                res.append(["5", "wrong columns in select!"])
                return res
            splt = splitSearch[1].strip().split(" ")
            if re.match("\s*\w+\s*", splt[0]):
                res.append([splt[0], "tablename1"])
            else:
                res.clear()
                res.append(["5", "wrong table name!"])
                return res
            if re.search("\s+(?i)join\s+",command) and re.search("(?i)where",command):
                if re.search("\s+(?i)join\s+", command).start() > re.search("(?i)where", command).start():
                    res.clear()
                    res.append(["5", "join after where!!"])
                    return res
            if re.search("\s+(?i)join\s+", command):
                command0 = command.strip()
                command0 = re.split("\s+(?i)join\s+", command0)[1].strip()
                command0 = re.split("\s+(?i)where\s+", command0)[0].strip()
                if re.match("\s*\w+(\s+((?i)on)\s+\w+\s+[=]\s+\w+\s*)\s*", command0):
                    if re.search("(?i)on", command0):
                        res.append([re.split("\s+(?i)on\s+", command0)[0], "tablename2"])
                        command0=re.split("\s+(?i)on\s+", command0)[1].strip()
                        cond = command0.split(" ")
                        res.append([cond[0], "join"])
                        res.append([cond[2], "join"])

                    else:
                        res.append([re.split("\s+(?i)on\s+", command0)[0], "tablename2"])
                        res.append(["$All", "join"])
                else:
                    res.clear()
                    res.append(["5", "bad join!"])
                    return res
            if re.search("\s+(?i)where\s+", command):
                command0 = command.strip()
                command0 = re.split("\s+(?i)where\s+", command0)[1].strip()
                if re.match('\s*[\w+]|["\w+"]\s+[=]|[!][=]|[>]|[<]|[>][=]|[<][=]\s+[\w+]|["\w+"]\s*', command0):
                    cond = command0.split(" ")
                    if re.match('"', cond[0]):
                        res.append([cond[0].replace('"',"").replace('"',""), "value"])
                    else:
                        res.append([cond[0], "colc"])
                    if re.match('"', cond[2]):
                        res.append([cond[2].replace('"',"").replace('"',""), "value"])
                    else:
                        res.append([cond[2], "colc"])
                    res.append([cond[1], "operationc"])
                else:
                    res.clear()
                    res.append(["5", "bad where!"])
                    return res
        else:
            res.clear()
            res.append(["5", "wrong usage of select!"])
            return res
    elif re.match("(?i)delete", splt[0]):
        res.append(["delete", "command"])
        command = command.replace(splt[0] + " ", "", 1)
        if re.match("(?i)from", splt[1]):
            splt = command.split(" ")
        if re.match("\s*\w+\s*",splt[1]):
            res.append([splt[1],"tablename"])
        else:
            res.clear()
            res.append(["5", "bad tablename!"])
            return res
        if re.search("\s+(?i)where\s+", command):
            command0 = command.strip()
            command0 = re.split("\s+(?i)where\s+", command0)[1]
            if re.match('\s*[\w+]|["\w+"]\s+[=]|[!][=]|[>]|[<]|[>][=]|[<][=]\s+[\w+]|["\w+"]\s*', command0):
                command0=command0.strip()
                cond = command0.split(" ")
                if re.match('"',cond[0]):
                    res.append([cond[0].replace('"',"").replace('"',""), "value"])
                else:
                    res.append([cond[0], "colc"])
                if re.match('"', cond[2]):
                    res.append([cond[2].replace('"',"").replace('"',""), "value"])
                else:
                    res.append([cond[2], "colc"])
                res.append([cond[1], "operationc"])
            else:
                res.clear()
                res.append(["5", "bad where!"])
                return res
    else:
        res.clear()
        res.append(["5", "unknown command!"])
    return res
