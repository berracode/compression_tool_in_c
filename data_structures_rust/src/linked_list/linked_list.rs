struct Node<T> {
    data: T,
    next: Option<Box<Node<T>>>,
}

impl<T> Node<T> {
    fn new(data: T) -> Node<T> {
        Node { data: data, next: None }
    }
}

pub struct LinkedList<T> {
    head: Option<Box<Node<T>>>,
}

impl<T> LinkedList<T> {
    pub fn new() -> LinkedList<T> {
        LinkedList { head: None }
    }

    pub fn is_empty(&self) -> bool {
        self.head.is_none()
    }

    pub fn push(&mut self, data: T) {
        let mut new_node = Box::new(Node::new(data));
        match self.head.take() {
            None => self.head = Some(new_node),
            Some(head) => {
                new_node.next = Some(head);
                self.head = Some(new_node);
            }
        }
    }

    pub fn pop(&mut self) -> Option<T> {
        self.head.take().map(|current_node| {
            self.head = current_node.next;
            current_node.data
        })
    }
}