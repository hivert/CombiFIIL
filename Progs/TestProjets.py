## Grammar examples

BinaryTrees = {
    "Tree": UnionRule("Node", "Leaf"),
    "Node": ProductRule("Tree", "Tree", lambda a,b: (a,b)),
    "Leaf": SingletonRule("Leaf")
}

Fibo = {
    "Fib": UnionRule("Vide", "Cas1"),
    "Cas1": UnionRule("CasAu", "Cas2"),
    "Cas2": UnionRule("AtomB","CasBAu"),
    "Vide": EpsilonRule(""),
    "AtomA": SingletonRule("A"),
    "AtomB": SingletonRule("B"),
    "CasAu": ProductRule("AtomA","Fib", lambda a,b: a+b),
    "CasBAu":ProductRule("AtomB","CasAu", lambda a,b: a+b)
}

BinaryWords = {
    "Word": UnionRule("Empty","NonEmpty"),
    "Empty" : EpsilonRule(""),
    "NonEmpty": ProductRule("Letter","Word",lambda a,b: a+b),
    "Letter": UnionRule("AtomA","AtomB"),
    "AtomA": SingletonRule("A"),
    "AtomB": SingletonRule("B")
}

DyckWords = {
    "DyckWord": UnionRule("Empty", "NonEmpty"),
    "Empty": EpsilonRule(""),
    "Peak": SingletonRule("10"),
    "Prim": ProductRule("Peak","DyckWord", lambda a,b: "1"+b+"0"),
    "NonEmpty": ProductRule("DyckWord","Prim", lambda a,b: a+b)
}

Palindromes = {
    "Palindrome": UnionRule("Empty", "NonEmpty"),
    "Empty": EpsilonRule(""),
    "A": SingletonRule("A"),
    "B": SingletonRule("B"),
    "Letter": UnionRule("A","B"),
    "Au" : ProductRule("A","Palindrome", lambda a,b: a+b),
    "AuA": ProductRule("Au","A",lambda a,b: a+b),
    "Bu": ProductRule("B","Palindrome", lambda a,b: a+b),
    "BuB": ProductRule("Bu","B", lambda a,b: a+b),
    "Double": UnionRule("AuA","BuB"),
    "NonEmpty": UnionRule("Double","Letter")
}

Binom = {
    "Binom": UnionRule("Empty", "NonEmpty"),
    "Empty": EpsilonRule(""),
    "A": SingletonRule("A"),
    "B": SingletonRule("B"),
    "Au" : ProductRule("A","Seq_AuB", lambda a,b: a+b),
    "AuB": ProductRule("Au","B", lambda a,b: a+b),
    "Seq_AuB": UnionRule("Empty", "Seq+_AuB"),
    "Seq+_AuB": ProductRule("AuB", "Seq_AuB", lambda a,b: a+b),
    "Bu" : ProductRule("B", "Seq_BuA", lambda a,b: a+b),
    "BuA": ProductRule("Bu","A", lambda a,b: a+b),
    "Seq_BuA": UnionRule("Empty", "Seq+_BuA"),
    "Seq+_BuA": ProductRule("BuA", "Seq_BuA", lambda a,b: a+b),
    "Prim": UnionRule("AuB","BuA"),
    "NonEmpty": ProductRule("Prim", "Binom", lambda a,b: a+b)
}

Pas3 = {
    "Pas3": UnionRule("Vide", "Cas"),
    "Cas": UnionRule("A0", "A1"),
    "Vide": EpsilonRule(""),
    "Atom0": SingletonRule("0"),
    "Atom1": SingletonRule("1"),
    "A0": ProductRule("Atom0", "EB0A1", lambda a,b: a+b),
    "EB0A1": UnionRule("Vide", "B0A1"),
    "B0A1": UnionRule("B0", "A1"),
    "A1": ProductRule("Atom1", "EB1A0", lambda a,b: a+b),
    "EB1A0": UnionRule("Vide", "B1A0"),
    "B1A0": UnionRule("B1", "A0"),
    "B0": ProductRule("Atom0", "EA1", lambda a,b: a+b),
    "EA1": UnionRule("Vide", "A1"),
    "B1": ProductRule("Atom1", "EA0", lambda a,b: a+b),
    "EA0": UnionRule("Vide", "A0"),
}

## Grammar examples using sequence and other extensions

Seq = {
    "Seq": SequenceRule("A","", lambda a,b: a+b),
    "A": SingletonRule("A")
}

BinaryWords2 = {
    "Word": SequenceRule("Letter", "", lambda a,b: a+b),
    "A": SingletonRule("A"),
    "B":SingletonRule("B"),
    "Letter": UnionRule("A","B")
}

MultiProd = {
    "A": SingletonRule("A"),
    "B" : SingletonRule("B"),
    "C" : SingletonRule("C"),
    "Prod" : MultiProductRule(("A","B","C"), lambda a,b: a+b)
}

Seqk = {
    "Seq": SequencekRule("A",3,"",lambda a,b: a+b),
    "A": SingletonRule("A")
}

Fibo2 = {
    "Fib": MultiUnionRule(("Vide", "CasAu", "AtomB","CasBAu")),
    "Vide": EpsilonRule(""),
    "AtomA": SingletonRule("A"),
    "AtomB": SingletonRule("B"),
    "CasAu": ProductRule("AtomA","Fib", lambda a,b: a+b),
    "CasBAu":ProductRule("AtomB","CasAu", lambda a,b: a+b)
}

Palindromes2 = {
    "Palindrome": MultiUnionRule(("Empty", "AuA", "BuB", "A", "B")),
    "Empty": EpsilonRule(""),
    "A": SingletonRule("A"),
    "B": SingletonRule("B"),
    "AuA" : MultiProductRule(("A","Palindrome", "A"), lambda a,b: a+b),
    "BuB" : MultiProductRule(("B","Palindrome", "B"), lambda a,b: a+b)
}

Binom2 = {
    "Binom": MultiUnionRule(("AuB+","BuA+","Empty")),
    "Empty": EpsilonRule(""),
    "A": SingletonRule("A"),
    "B": SingletonRule("B"),
    "AuB": MultiProductRule(("A","Seq_AuB","B"), lambda a,b: a+b),
    "BuA": MultiProductRule(("B","Seq_BuA","A"), lambda a,b: a+b),
    "Seq_AuB": SequenceRule("AuB", "", lambda a,b: a+b),
    "Seq_BuA": SequenceRule("BuA", "", lambda a,b: a+b),
    "AuB+": ProductRule("AuB","Binom", lambda a,b: a+b),
    "BuA+": ProductRule("BuA","Binom", lambda a,b: a+b),
}

PlanarTrees = {
    "Tree": ProductRule("Node","Subtrees", lambda a,b: (a,b)),
    "Node": SingletonRule("Node"),
    "Subtrees": SequenceRule("Tree",tuple(), lambda a,b: (a,) + b)
}

def mAryTrees(m):
    return {
        "Tree": UnionRule("Leaf","Node+"),
        "Leaf": EpsilonRule("Leaf"),
        "Node" : SingletonRule("Node"),
        "Children": SequencekRule("Tree",m,tuple(), lambda a,b: (a,)+b),
        "Node+": ProductRule("Node","Children", lambda a,b: (a,b))
    }
