use data_structures_rust::linked_list::linked_list_with_rc_ref_cell::LinkedList;
use data_structures_rust::linked_list::linked_list::LinkedList as LinkedList2;
fn main() {
    let mut list = LinkedList2::new();

    list.push(1);
    list.push(2);

    println!("{:?}", list.pop());

  
}