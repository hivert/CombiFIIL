
class LabelledBinaryTree():

    def __init__(self, children = None, label = None):
        """
        A binary tree is either a leaf or a node with two subtrees.

        INPUT:

            - children, either None (for a leaf), or a list of size excatly 2
            of either two binary trees or 2 objects that can be made into binary trees
            - label, the node label
        """
        self._isleaf = (children is None)
        if not self._isleaf:
            if len(children) != 2:
                raise ValueError("A binary tree needs exactly two children")
            self._children = tuple(c if isinstance(c,LabelledBinaryTree) else LabelledBinaryTree(c) for c in children)
        self._size = None
        self._label = label

    def label(self):
        """
        Return the node label
        """
        return self._label

    def __repr__(self):
        s = "" if self._label is None else self._label
        if self.is_leaf():
            if self._label != None:
                return str(s)
            return "leaf"
        return str(s) + str(self._children)

    def __eq__(self, other):
        """
        Return true if other represents the same binary tree as self
        """
        if not isinstance(other, LabelledBinaryTree):
            return False
        if self.is_leaf():
            return other.is_leaf() and other.label() == self.label()
        return self.left() == other.left() and self.right() == other.right()


    def left(self):
        """
        Return the left subtree of self
        """
        return self._children[0]

    def right(self):
        """
        Return the right subtree of self
        """
        return self._children[1]

    def is_leaf(self):
        """
        Return true is self is a leaf
        """
        return self._isleaf

    def _compute_size(self):
        """
        Recursively computes the size of self
        """
        if self.is_leaf():
            self._size = 0
        else:
            self._size = self.left().size() + self.right().size() +1

    def size(self):
        """
        Return the number of nodes (non leaves) in the binary tree
        """
        if self._size is None:
            self._compute_size()
        return self._size


def Node(t1,t2, label = None):
    return LabelledBinaryTree([t1,t2], label)

def Leaf(label = None):
    return LabelledBinaryTree(label = label)
