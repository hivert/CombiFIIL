from projet import *
from LabelledBinaryTree import Node, Leaf

# Exemple Grammaires

## Grammaires triviales

### Grammaire Singleton

GSingl = {
    "Singl": SingletonRule(lambda x:x)
}

init_grammar(GSingl)
assert GSingl["Singl"].count(0) == 0 # nb d'objets de taille 0
assert GSingl["Singl"].count(1) == 1 # nb d'objets de taille 1
assert GSingl["Singl"].count(2) == 0 # nb d'objets de taille 2

# liste des objets de taille 0
assert GSingl["Singl"].list([]) == []
# liste des objets étiquetés par un ensemble donné
assert GSingl["Singl"].list(["a"]) == ["a"]
assert GSingl["Singl"].list(["b"]) == ["b"]
assert GSingl["Singl"].list(["a","b"]) == []

### Grammaire Epsilon

GMotVide = {
    "Empty": EpsilonRule("")
}

init_grammar(GMotVide)
assert GMotVide["Empty"].count(0) == 1 # nb d'objets de taille 0
assert GMotVide["Empty"].count(1) == 0 # nb d'objets de taille 1
assert GMotVide["Empty"].count(2) == 0 # nb d'objets de taille 2

# liste des objets de taille 0
assert GMotVide["Empty"].list([]) == [""]
# liste des objets étiquetés par un ensemble donné
assert GMotVide["Empty"].list(["a"]) == []
assert GMotVide["Empty"].list(["b"]) == []
assert GMotVide["Empty"].list(["a","b"]) == []

## Séquences

### Permutations de taille 2

Perm2 = {
    "Perms": ProductRule("Letter","Letter", lambda a,b: [a,b]),
    "Letter": SingletonRule(lambda x:x)
}

init_grammar(Perm2)
assert Perm2["Perms"].count(0) == 0 # nb d'objets de taille 0
assert Perm2["Perms"].count(1) == 0 # nb d'objets de taille 1
assert Perm2["Perms"].count(2) == 2 # nb d'objets de taille 2

# liste des objets de taille 0
assert Perm2["Perms"].list([]) == []
# liste des objets étiquetés par un ensemble donné
assert Perm2["Perms"].list(["a"]) == []
assert Perm2["Perms"].list(["b"]) == []
assert Perm2["Perms"].list(["a","b"]) == [["a","b"], ["b","a"]]
assert Perm2["Perms"].list([1,2]) == [[1,2], [2,1]]

### Permutations

Perms = {
    "Perms": UnionRule("Empty","NonEmpty"),
    "NonEmpty": ProductRule("Letter","Perms",lambda l1,l2:l1+l2),
    "Letter": SingletonRule(lambda x:[x]),
    "Empty": EpsilonRule([])
}

init_grammar(Perms)
assert Perms["Perms"].count(0) == 1 # nb d'objets de taille 0
assert Perms["Perms"].count(1) == 1 # nb d'objets de taille 1
assert Perms["Perms"].count(2) == 2 # nb d'objets de taille 2
assert Perms["Perms"].count(3) == 6 # nb d'objets de taille 3
assert Perms["Perms"].count(4) == 24 # nb d'objets de taille 4

# liste des objets de taille 0
assert Perms["Perms"].list([]) == [[]]
# liste des objets étiquetés par un ensemble donné
assert Perms["Perms"].list(["a"]) == [["a"]]
assert Perms["Perms"].list(["b"]) == [["b"]]
assert Perms["Perms"].list(["a","b"]) == [["a","b"], ["b","a"]]
assert Perms["Perms"].list([1,2]) == [[1,2], [2,1]]
assert Perms["Perms"].list([1,2,3]) == [
[1,2,3], [1,3,2], [2,1,3], [2,3,1], [3,1,2], [3,2,1]]

### Sequences ordonnées

SortedSeq = {
    "Sorted": UnionRule("Empty","NonEmpty"),
    "NonEmpty": OrdProdRule("Letter","Sorted",lambda l1,l2:l1+l2),
    "Letter": SingletonRule(lambda x:[x]),
    "Empty": EpsilonRule([])
}

init_grammar(SortedSeq)
assert SortedSeq["Sorted"].count(0) == 1 # nb d'objets de taille 0
assert SortedSeq["Sorted"].count(1) == 1 # nb d'objets de taille 1
assert SortedSeq["Sorted"].count(2) == 1 # nb d'objets de taille 2
assert SortedSeq["Sorted"].count(3) == 1 # nb d'objets de taille 3
assert SortedSeq["Sorted"].count(4) == 1 # nb d'objets de taille 4

# liste des objets de taille 0
assert SortedSeq["Sorted"].list([]) == [[]]
# liste des objets étiquetés par un ensemble donné
assert SortedSeq["Sorted"].list(["a"]) == [["a"]]
assert SortedSeq["Sorted"].list(["b"]) == [["b"]]
assert SortedSeq["Sorted"].list(["a","b"]) == [["a","b"]]
assert SortedSeq["Sorted"].list([1,2]) == [[1,2]]
assert SortedSeq["Sorted"].list([1,2,3]) == [[1,2,3]]

### Exemple Boxed product

ExBoxProduct = {
    "BoxProduct": BoxProdRule("Sorted", "Sorted", lambda a,b: (a,b)),
    "Sorted": UnionRule("Empty","NonEmpty"),
    "NonEmpty": OrdProdRule("Letter","Sorted",lambda l1,l2:l1+l2),
    "Letter": SingletonRule(lambda x:[x]),
    "Empty": EpsilonRule([])
}

init_grammar(ExBoxProduct)
assert ExBoxProduct["BoxProduct"].count(0) == 0 # nb d'objets de taille 0
assert ExBoxProduct["BoxProduct"].count(1) == 1 # nb d'objets de taille 1
assert ExBoxProduct["BoxProduct"].count(2) == 2 # nb d'objets de taille 2
assert ExBoxProduct["BoxProduct"].count(3) == 4 # nb d'objets de taille 3

# liste des objets de taille 0
assert ExBoxProduct["BoxProduct"].list([]) == []
# liste des objets étiquetés par un ensemble donné
assert ExBoxProduct["BoxProduct"].list(["a"]) == [(["a"],[])]
assert ExBoxProduct["BoxProduct"].list([1,2]) == [
([1],[2]), ([1,2],[])]
assert ExBoxProduct["BoxProduct"].list([1,2,3]) == [
([1],[2,3]), ([1,2], [3]), ([1,3], [2]), ([1,2,3],[])]

## Arbres binaires

### Arbres binaires avec labels feuilles

from LabelledBinaryTree import Node, Leaf

TreeLabelLeaves = {
    "Tree": UnionRule("Node", "Leaf"),
    "Node" : ProductRule("Tree", "Tree", Node),
    "Leaf" : SingletonRule(Leaf)
}

init_grammar(TreeLabelLeaves)

assert TreeLabelLeaves["Tree"].count(0) == 0
assert TreeLabelLeaves["Tree"].count(1) == 1
assert TreeLabelLeaves["Tree"].count(2) == 2
assert TreeLabelLeaves["Tree"].count(3) == 12 # 2 * 6
assert TreeLabelLeaves["Tree"].count(4) == 120 # 5 * 24

assert TreeLabelLeaves["Tree"].list(["a","b"]) == [
Node(Leaf("a"),Leaf("b")), Node(Leaf("b"),Leaf("a"))]
assert len(TreeLabelLeaves["Tree"].list(["a","b","c"])) == 12
assert len(TreeLabelLeaves["Tree"].list(["a","b","c","d"])) == 120

### Arbres binaires avec labels noeuds

TreeLabelNodes = {
    "Tree" : UnionRule("Node", "Leaf"),
    "Node" : ProductRule("Label","Subtrees", lambda l,t: Node(t[0],t[1],l)),
    "Label" : SingletonRule(lambda x:x),
    "Subtrees" : ProductRule("Tree","Tree", lambda t1,t2: (t1,t2)),
    "Leaf" : EpsilonRule(Leaf())
}

init_grammar(TreeLabelNodes)

assert TreeLabelNodes["Tree"].count(0) == 1
assert TreeLabelNodes["Tree"].count(1) == 1
assert TreeLabelNodes["Tree"].count(2) == 4 # 2 * 2
assert TreeLabelNodes["Tree"].count(3) == 30 # 5 * 6
assert TreeLabelNodes["Tree"].count(4) == 336 # 14 * 24

assert len(TreeLabelNodes["Tree"].list(["a","b","c"])) == 30



