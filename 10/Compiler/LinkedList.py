import Node

class LinkedList:

  def __init__(self, head = None):
    self.head = head
    self.count = 0

  def insert(self, data):
    """
    Create a new node at the Head of the Linked List
    """

    #create a new node to hold the data
    new_node = Node.Node(data)

    #set the next of the new node to the current head
    new_node.set_next(self.head)

    #set the head of the Linked List to the new head
    self.head = new_node

    self.count += 1


  def pop(self):
    """
    Remove head of list
    """

    #set the head of the Linked List to the next node
    self.head = self.head.get_next()

    self.count -= 1


  def match(self, token):
    return self.head.val.get_data() == token.get_data()


  def get_count(self):
    return self.count


  def is_empty(self):
    return self.head == None


  def __str__(self):
    token = self.head
    str_list = str(token.get_data())
    while token.get_next():
      token = token.get_next()
      str_list += "\n" + str(token.get_data())

    return str_list