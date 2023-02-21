use data_structures_rust::linked_list::linked_list_with_rc_ref_cell::LinkedList;

fn main() {
    let mut list = LinkedList::new();

    list.push_back(1);
    list.push_back(2);
    list.push_front(3);

    println!("{:?}", list.pop_front()); // Some(3)
    println!("{:?}", list.pop_front()); // Some(1)
}