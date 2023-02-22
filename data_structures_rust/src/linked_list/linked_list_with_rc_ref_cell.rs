use std::cell::RefCell;
use std::rc::Rc;

#[derive(Debug)]
struct Node<T> {
    value: T,
    next: RefCell<Option<Rc<Node<T>>>>,
}

impl<T> Node<T> {
    fn new(value: T) -> Rc<Self> {
        Rc::new(Self {
            value,
            next: RefCell::new(None),
        })
    }
}

#[derive(Debug)]
pub struct LinkedList<T> {
    head: Option<Rc<Node<T>>>,
    tail: Option<Rc<Node<T>>>,
    length: usize,
}

impl<T> LinkedList<T> {
    pub fn new() -> Self {
        Self {
            head: None,
            tail: None,
            length: 0,
        }
    }

    pub fn push_front(&mut self, value: T) {
        let new_node = Node::new(value);

        if let Some(head) = self.head.take() {
            new_node.next.borrow_mut().replace(head);
            self.head = Some(new_node);
        } else {
            self.head = Some(new_node.clone());
            self.tail = Some(new_node);
        }

        self.length += 1;
    }

    pub fn push_back(&mut self, value: T) {
        let new_node = Node::new(value);

        if let Some(tail) = self.tail.take() {
            tail.next.borrow_mut().replace(new_node.clone());
            self.tail = Some(new_node);
        } else {
            self.head = Some(new_node.clone());
            self.tail = Some(new_node);
        }

        self.length += 1;
    }

    pub fn pop_front(&mut self) -> Option<T> {
        self.head.take().map(|head| {
            if let Some(next) = head.next.borrow_mut().take() {
                self.head = Some(next);
            } else {
                self.tail.take();
            }

            self.length -= 1;

            Rc::try_unwrap(head).ok().unwrap().value
        })
    }

    /*fn pop_back(&mut self) -> Option<T> {
        self.tail.take()
        .map(|tail| {
            if let Some(head) = self.head.as_ref() {
                if Rc::ptr_eq(head, &tail) {
                    self.head.take();
                } else {
                    let mut node = head;
                    while !Rc::ptr_eq(node, &tail) {
                        node = node.next.borrow_mut().as_ref().unwrap_or_else(||{
                            println!("er");
                        });
                    }

                    
                    
                    
                }
            } else {
                self.head.take();
            }
    
            self.length -= 1;
    
            Rc::try_unwrap(tail).ok().unwrap().value
        })
    }*/
}

