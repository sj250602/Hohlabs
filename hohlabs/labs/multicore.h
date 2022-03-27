#pragma once
#include<atomic>

//
// INVARIANT: w_deleted_count <= w_deleting_count <= w_cached_read_count <= shared_read_count <= r_reading_count <= r_cached_write_count <= shared_write_count <= w_writing_count <= w_deleted_count + MAX_SIZE
//
// INVARIANT:      w_writing_count      - w_deleted_count     <= MAX_SIZE
// =========>      w_writing_count      - w_cached_read_count <= MAX_SIZE
// =========>      shared_write_count   - w_cached_read_count <= MAX_SIZE
// =========>      shared_write_count   - shared_read_count   <= MAX_SIZE
//
//
// INVARIANT: 0 <= r_cached_write_count - r_reading_count
// =========> 0 <= r_cached_write_count - shared_read_count
// =========> 0 <= shared_write_count   - shared_read_count
//
//
// THEOREM: =========> 0 <= shared_write_count   - shared_read_count   <= MAX_SIZE
//





//
//
// Channel/Queue:
//
// Shared between Producer and Consumer
//
struct channel_t{
  public:

    //insert your code here
    std::atomic<int> producer; //write
    std::atomic<int> consumer; //read
    std::atomic<int> isfull; //read
    std::atomic<int> isempty; //write

  public:

    //
    // Intialize
    //
    channel_t(){

      // insert your code here
      
      producer = 0;
      consumer = 0;
      isfull = 0;
      isempty = 1;
      

    }
};


//
// Producer's (not shared)
//
struct writeport_t{
public:
    //insert your code here
    size_t qsize;
    int pfront;
    int prear;
    int cfront;
    int queue_is_full;
    int queue_is_empty;

public:

  //
  // Intialize
  //
  writeport_t(size_t tsize)
  {
    //insert code here
    qsize = tsize;
    pfront = 0;
    prear = 0;
    cfront = 0;
    queue_is_full = 0;
    queue_is_empty = 1;
  }

public:

  //
  // no of entries available to write
  //
  // helper function for write_canreserve
  //
  size_t write_reservesize(){

    // insert your code here
    if(queue_is_empty==1){
    	return qsize;
    }
    return (size_t)(((int)qsize-(pfront-prear))%(int)qsize);	
  }

  //
  // Can write 'n' entries?
  //
  bool write_canreserve(size_t n){

    // insert your code here
    return n<=write_reservesize();
  }

  //
  // Reserve 'n' entries for write
  //
  size_t write_reserve(size_t n){
    // insert your code here
    if(n!=0 && queue_is_empty==1){
    	queue_is_empty = 0;
    }
    int front_old = pfront;
	pfront = pfront + ((int)n);
	pfront = pfront % ((int)qsize);
	if(pfront==prear){
		queue_is_full = 1;
	}
	return (size_t)front_old;
  }

  //
  // Commit
  //
  // Read/Write shared memory data structure
  //
  void write_release(channel_t& ch){

    // insert your code here
    ch.producer = pfront;
    ch.isfull = queue_is_full;
    ch.isempty = queue_is_empty;

  }




public:

  //
  //
  // Read/Write shared memory data structure
  //
  void read_acquire(channel_t& ch){

    //insert your code here
    cfront = ch.consumer;

  }




  //
  // No of entires available to delete
  //
  size_t delete_reservesize(){
    //insert your code here
    if(queue_is_full==1){
    	return qsize;
    }
    return (size_t)(((int)qsize-(prear-cfront))%(int)qsize);	
  	  
  }

  //
  // Can delete 'n' entires?
  //
  bool delete_canreserve(size_t n){
    //insert your code here
    
    return n<=delete_reservesize();
  }

  //
  // Reserve 'n' entires for deletion
  //
  size_t delete_reserve(size_t n){
    //insert your code here
    if(n!=0 && queue_is_full){
    	queue_is_full = 0;
    }
    int rear_old = prear;
	prear = prear + ((int)n);
	prear = prear % ((int)qsize);
	if(prear==cfront){
		queue_is_empty =1;
	}
	return rear_old;
  }


  //
  // Update the state, if any.
  //
  void delete_release(){
    //insert your code here
	return;
  }


};


//
// Consumer's (not shared)
//
//
struct readport_t{
public:

  //insert your code here
	size_t qsize;
    int cfront;
    int crear;
    int queue_is_full;
    int queue_is_empty;


public:
  //
  // Initialize
  //
  readport_t(size_t tsize)
  {

    //insert your code here
    qsize = tsize;
    cfront = 0;
    crear = 0;
    queue_is_full = 0;
    queue_is_empty = 0;


  }
  public:

  //
  // Read/Write shared memory data structure
  //
  void write_acquire(channel_t& ch){

    //insert your code here
    cfront = ch.producer;
    queue_is_full = ch.isfull;
    queue_is_empty = ch.isempty;


  }

  //
  // no of entries available to read
  //
  size_t read_reservesize(){

    //insert your code here
    if(queue_is_full==1){
    	return qsize;
    }
    return (size_t)(((int)qsize-(crear-cfront))%(int)qsize);
    
  }

  //
  // Can Read 'n' entires?
  //
  bool read_canreserve(size_t n){

    //insert your code here

    return n<=read_reservesize();
  }

  //
  // Reserve 'n' entires to be read
  //
  size_t read_reserve(size_t n){

    //insert your code here
    if(n!=0 && queue_is_full==1){
    	queue_is_full = 0;
    }
    int rear_old = crear;
	crear = crear + ((int)n);
	crear = crear % ((int)qsize);
	if(cfront==crear){
		queue_is_empty = 1;
	}
	return rear_old;
	
  }

  //
  // Read/write shared memory data structure
  //
  void read_release(channel_t& ch){

    //insert your code here
    ch.consumer = cfront;

  }

};
