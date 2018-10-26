
class AbstractRule():
    """
    Classe abstraite pour tous les ensembles engendrés par une grammaire.
    """


    def __init__(self):
        """
        Méthode d'initialisation pour tous les ensembles
        """

    def valuation(self):
        return self._valuation



class ConstantRule(AbstractRule):
    """
    Représente un ensemble composé d'un objet unique dont la taille
    est spécifié par la méthode degree
    """

    def degree(self):
        raise NonImplementedError



class SingletonRule(ConstantRule):
    """
    Représente un ensemble composé d'un objet unique de taille 1
    """
    def __init__(self, fun):
        """
        Input :
            - fun, la fonction qui construit l'objet depuis l'etiquette
        """
        ConstantRule.__init__(self) # initialisation de la super classe
        self._fun = fun

    def __repr__(self):
        return "Singleton"

    def fun(self, x):
        """
        Retourne l'objet unique associé à l'ensemble

        Input :

            - x, une étiquette
        """
        return self._fun(x)


class EpsilonRule(ConstantRule):
    """
    Représente un ensemble composé d'un objet unique de taille 0
    """
    def __init__(self, obj):
        """
        Input :
            - obj, l'objet unique appartement à l'ensemble
        """
        AbstractRule.__init__(self) # initialisation de la super classe
        self._obj = obj

    def obj(self):
        """
        Retourne l'objet unique associé à l'ensemble
        """
        return self._obj

    def __repr__(self):
        return "Epsilon " + str(self.obj())


class ConstructorRule(AbstractRule):
    """
    Représente un ensemble d'objets construit à partir d'autres ensembles
    """

    def __init__(self, parameters):
        """
        Input :
            - parameters, un tuple contenant les clés identifiant les ensembles nécessaires à l'ensemble construit
        """
        AbstractRule.__init__(self)  # initialisation de la super classe
        self._parameters = parameters

    def parameters(self):
        """
        Retourne les paramètres du constructeurs : les clés des ensembles nécessaires à l'ensemble construit
        """
        return self._parameters



class UnionRule(ConstructorRule):
    """
    Représente un ensemble union de deux autres ensembles
    """

    def __init__(self, key1, key2):
        """
        Input :
            - key1, la clé du premier ensemble de l'union
            - key2, la clé du second ensemble de l'union
        """
        ConstructorRule.__init__(self,(key1,key2))

    def __repr__(self):
        return "Union of " + str(self._parameters)


class AbstractProductRule(ConstructorRule):
    """
    Représente un ensemble produit de deux autres ensembles
    """

    def __init__(self, key1, key2, cons):
        """
        Input :
            - key1, la clé du premier ensemble du produit
            - key2, la clé du second ensemble du produit
            - cons une fonction prenant deux paramètres : ``òbj1`` un objet de l'ensemble ``key1``
            et ``obj2`` un objet de l'ensemble ``key2``, et renvoyant un objet de l'ensemble produit
        """
        ConstructorRule.__init__(self,(key1,key2))
        self._cons = cons

    def construct(self, obj1, obj2):
        return self._cons(*(obj1,obj2))



class OrdProdRule(AbstractProductRule):
    """
    Représente un ensemble produit de deux autres ensembles avec labels ordonnés
    """
    def __repr__(self):
        return "Ordered Product of " + str(self.parameters())


class ProductRule(AbstractProductRule):
    """
    Représente un ensemble produit de deux autres ensembles
    """

    def __repr__(self):
        return "Product of " + str(self.parameters())



class BoxProdRule(AbstractProductRule):
    """
    Représente un ensemble produit de deux autres ensembles avec plus petit
    label à gauche
    """

    def __repr__(self):
        return "Boxed Product of " + str(self.parameters())





def save_grammar(gram):
    """
    Parcourt les ensembles de la grammaires et leur associe le dictionnaire (clé, ensemble)
    qui constitue la grammaire.

    Input :

        - gram, une grammaire donnée sous forme d'un dictionnaire
    """



def check_grammar(gram):
    """
    Retourne vrai si toutes les clés utilisées dans les ConstructorRule
    appartiennent bien au dictionnaire de la grammaire.
    """

def init_grammar(gram):
    """
     * Utilise la fonction save_grammar pour enregistrer la grammaire au
       niveau des différents ensembles
     * Vérifie la cohérence de la grammaire avec la fonction check_grammar
       (lève une exception si la fonction renvoie faux)
     * Calcul la valuation sur les ensembles de la grammaire

    Input :

        - gram, une grammaire donnée sous forme d'un dictionnaire clé - ensembles
    """


