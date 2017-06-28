class Node:
    def __init__(self, key):
        self.left = None
        self.right = None
        self.parent = None
        self.key = key
        self.numBelow = 1
        self.height = 0

    def __str__(self):
        return str(self.key)

class Tree:
    def __init__(self):
        self.root = None

    def getRoot(self):
        return self.root

    def insert(self, key):
        if self.root == None:
            self.root = Node(key)
        else:
            self._insert(self.root, key)

    def _insert(self, node, key):
        node.numBelow += 1
        if node.key > key:
            if node.left != None:
                self._insert(node.left, key)
            else:
                node.left = Node(key)
                node.left.parent = node

                self.rebalance(node)
        else:
            if node.right != None:
                self._insert(node.right, key)
            else:
                node.right = Node(key)
                node.right.parent = node

                self.rebalance(node)

    def inorder(self):
        L = []
        self._inorder(self.root, L)
        return L

    def _inorder(self, node, outL):
        if node != None:
            self._inorder(node.left, outL)
            outL.append(node.key)
            self._inorder(node.right, outL)

    def find(self, key):
        return self._find(self.root, key)

    def _find(self, node, key):
        if node == None:
            return None

        if node.key == key:
            return node
        elif node.key > key:
            return self._find(node.left, key)
        else:
            return self._find(node.right, key)

    def delete(self, key):
        node = self.find(key)
        self._delete(node)

    def _delete(self, node):
        # Case 1: no child
        if node == None or (node.left is None and node.right is None):
            if node == self.root:
                self.root = None
            elif node.parent.left == node: node.parent.left = None
            else: node.parent.right = None

            self.rebalance(node.parent)
            return
        # Case 2: One of them is null
        elif node.left is None or node.right is None:
            parent = node.parent
            remainingNode = node.left if node.left is not None else node.right
            if parent is not None:
                isLeftNode = parent.left == node
                remainingNode.parent = parent

                if isLeftNode: parent.left = remainingNode
                else: parent.right = remainingNode
                #del remainingNode
            else:
                root = remainingNode

            self.rebalance(remainingNode)
        # Case 3: both nodes are not null
        else:
            successor = self.successor(node)
            node.key, successor.key = successor.key, node.key
            self._delete(successor)

    def findMin(self, node):
        if node.left is not None: return self.findMin(node.left)
        else: return node

    def findMax(self, node):
        if node.right is not None: return self.findMax(node.right)
        else: return node

    def successor(self, node):
        if node.right is not None:
            return self.findMin(node.right)
        else:
            currNode = node
            currParent = node.parent

            # return the first parent whom is a left child
            while currParent != None and currParent.left != currNode:
                currNode = currParent
                currParent = currNode.parent

            return currParent


    def _getNumSmallerOrEqualTo(self, node, key):
        if node == None:
            return 0

        if node.key <= key:
            return (1 + (node.left.numBelow if node.left is not None else 0)
                      + self._getNumSmallerOrEqualTo(node.right, key))
        else:
            return self._getNumSmallerOrEqualTo(node.left, key)

    def getNumSmallerOrEqualTo(self, key):
        return self._getNumSmallerOrEqualTo(self.root, key)

    def rotate_left(self, X):
        xParent = X.parent
        isLeftChild = False if xParent is None or xParent.left != X else True
        y = X.right
        assert (y is not None)
        X.right = y.left
        if y.left is not None: y.left.parent = X
        y.left = X
        y.parent = xParent
        X.parent = y

        if xParent:
            if isLeftChild: xParent.left = y
            else: xParent.right = y
        else:
            self.root = y

        update_height(X)
        update_height(y)

    def rotate_right(self, X):
        xParent = X.parent
        isLeftChild = False if xParent is None or xParent.left != X else True
        y = X.left
        assert(y is not None)

        X.left = y.right
        if y.right is not None: y.right.parent = X.left
        y.right = X
        y.parent = xParent
        X.parent = y

        if xParent:
            if isLeftChild: xParent.left = y
            else: xParent.right = y
        else:
            self.root = y

    def rebalance(self, node):
        while node is not None:
            # rebalance code
            oldHeight = node.height
            update_height(node)
            newHeight = node.height

            # No reason to continue looping if the height didn't changed
#            if oldHeight == newHeight:
#                break;

            if height(node.right) >= 2 + height(node.left):
                if height(node.right.right) >= height(node.right.left): # Case 1
                    self.rotate_left(node)
                else:
                    self.rotate_right(node.right)
                    self.rotate_left(node)

                node = node.parent.parent # Because node falls down one level
            elif height(node.left) >= 2 + height(node.right):
                if height(node.left.left) >= height(node.left.right):
                    self.rotate_right(node)
                else:
                    self.rotate_left(node.left)
                    self.rotate_right(node)

                node = node.parent.parent  # Because node falls down one level
            else:
                node = node.parent

    def _FindKth(self, node, k):
        if node is None:
            return None
        assert(k > 0)

        nl = getNumBelow(node.left)
        if nl == k - 1:
            return node
        if nl < k-1:
            return self._FindKth(node.right, k - nl - 1)
        else:
            return self._FindKth(node.left, k)

    def FindKth(self, k):
        return self._FindKth(self.root, k)

def height(node):
    if node is None: return -1
    else: return node.height

def getNumBelow(node):
    if node is None: return 0
    else: return node.numBelow

def update_height(node):
    node.height = max(height(node.left), height(node.right)) + 1
    node.numBelow = getNumBelow(node.left) + getNumBelow(node.right) + 1

if __name__ == '__main__':
    values = [49, 46, 43, 79, 64, 83]
    t = Tree()
    for i in values:
        t.insert(i)

    #t.delete(49)

    print("element requested is " + str(t.FindKth(6).key))

    print (t.getNumSmallerOrEqualTo(80))