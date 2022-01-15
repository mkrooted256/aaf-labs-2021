class Tree:
    def __init__(self ):
        self.right = None
        self.node = None
        self.left = None
        self.row = None

    def InsertIndex(self, str, row):
        if self.node is None:
            self.node = str
            self.row = row
            return
        else:
            if self.node <= str:
                if self.right is None:
                    self.right = Tree()
                self.right.InsertIndex(str, row)
            else:
                if self.left is None:
                    self.left = Tree()
                self.left.InsertIndex(str, row)

    def NEq(self, array, str):
        if self.node < str:
            array.append(self.row)
            if self.right is not None:
                self.right.NEq(array, str)
        elif self.node > str:
            array.append(self.row)
            if self.right is not None:
                self.right.NEq(array, str)
            if self.left is not None:
                self.left.NEq(array, str)
        else:

            if self.right is not None:
                self.right.NEq(array, str)

    def Eq(self, array, str):
        if self.node < str:
            if self.right is not None:
                self.right.Eq(array, str)
        elif self.node > str:
            if self.left is not None:
                self.left.Eq(array, str)
        else:
            array.append(self.row)
            if self.right is not None:
                self.right.Eq(array, str)

    def More(self, array, str):
        if self.node > str:
            array.append(self.row)
            if self.right is not None:
                self.right.More(array, str)
            if self.left is not None:
                self.left.More(array, str)
        elif self.node < str:

            if self.right is not None:
                self.right.More(array, str)
        else:
            if self.right is not None:
                self.right.More(array, str)

    def MoreEq(self, array, str):
        if self.node >= str:
            array.append(self.row)
            if self.right is not None:
                self.right.MoreEq(array, str)
            if self.left is not None:
                self.left.MoreEq(array, str)
        elif self.node < str:

            if self.right is not None:
                self.right.MoreEq(array, str)

    def Less(self, array, str):
        if self.node < str:
            array.append(self.row)
            if self.right is not None:
                self.right.Less(array, str)
            if self.left is not None:
                self.left.Less(array, str)
        elif self.node > str:

            if self.left is not None:
                self.left.Less(array, str)
        else:
            if self.right is not None:
                self.right.Less(array, str)

    def LessEq(self, array, str):
        if self.node < str:
            array.append(self.row)
            if self.right is not None:
                self.right.LessEq(array, str)
            if self.left is not None:
                self.left.LessEq(array, str)
        elif self.node > str:

            if self.left is not None:
                self.left.LessEq(array, str)
        else:
            array.append(self.row)
            if self.right is not None:
                self.right.LessEq(array, str)

    def deleteRe(self, right, left, new):
        if right is not None:
            new.InsertIndex(right.node, right.row)
            new.deleteRe(right.right, right.left, new)
        if left is not None:
            new.InsertIndex(left.node, left.row)
            new.deleteRe(left.right, left.left, new)

    def DeleteIndex(self, row, str):
        if self.node == str and self.row == row:
            if self.right is None and self.left is None:

                del self
                return None

            elif self.right is None and self.left is not None:

                return self.left
            elif self.right is not None and self.left is None:
                return self.right
            else:
                new = Tree()

                self.deleteRe(self.right, self.left, new)
                return new
        elif self.node < str or (self.node == str and self.row != row):
            if self.right is not None:
                self.right = self.right.DeleteIndex(row, str)
        elif self.node > str:
            if self.left is not None:
                self.left = self.left.DeleteIndex(row, str)
        return self
