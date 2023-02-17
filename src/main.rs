fn main() {
    let mut list = LinkedList::new();

    list.push_back(1);
    list.push_back(2);
    list.push_front(3);

    println!("{:?}", list.pop_back()); // Some(2)
    println!("{:?}", list.pop_front()); // Some(3)
    println!("{:?}", list.pop_front()); // Some(1)
    println!("{:?}", list.pop_back()); // None
}