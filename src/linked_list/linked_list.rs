struct Node<T> {
    data: T,
    next: Option<Box<Node<T>>>,
}

impl<T> Node<T> {
    fn new(data: T) -> Node<T> {
        Node { data: data, next: None }
    }
}

struct LinkedList<T> {
    head: Option<Box<Node<T>>>,
}

impl<T> LinkedList<T> {
    fn new() -> LinkedList<T> {
        LinkedList { head: None }
    }

    fn is_empty(&self) -> bool {
        self.head.is_none()
    }

    fn push(&mut self, data: T) {
        let new_node = Box::new(Node::new(data));
        match self.head.take() {
            None => self.head = Some(new_node),
            Some(head) => {
                new_node.next = Some(head);
                self.head = Some(new_node);
            }
        }
    }

    fn pop(&mut self) -> Option<T> {
        self.head.take().map(|head| {
            self.head = head.next;
            head.data
        })
    }
}