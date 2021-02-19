package ceng.ceng351.labdb;

public class Bucket {
    private int local_depth;
    private int bucket_size;
    private String[] bucket;
    private int no_elements;
    
    public Bucket(int size){
         this.local_depth = 1;
         this.bucket_size = size;
         this.bucket = new String[this.bucket_size];
         this.no_elements = 0;
    }

    public Bucket(int size, int local_depth){
          this.local_depth = local_depth;
          this.bucket_size = size;
          this.bucket = new String[this.bucket_size];
          this.no_elements = 0;
    }

    public int getLocalDepth(){
         return local_depth;
    }

    public void incrementLocalDepth(){
         this.local_depth += 1;
    }

    public void decrementLocalDepth(){
          this.local_depth -= 1;
     }

    public void setLocalDepth(int new_depth){
          this.local_depth = new_depth;
    }

    public boolean addElement(String id){
         boolean flag = true;
          if(isOverflowed()){
               return false;
          }
          for(int i = 0; i < no_elements; i++){
               if(bucket[i].equals(id)){
                    flag = false;
                    break;
               }
          }
          if(flag){
               bucket[no_elements] = id;
               this.no_elements += 1;
          }
          return true;
    }

    public String getElement(int index){
          return bucket[index];
    }

    public int getNoElements(){
         return no_elements;
    }

    public void setNoElements(int no){
          this.no_elements = no;
    }

    public boolean deleteElement(String id){
         for(int i = 0; i < bucket_size; i++){
              if(bucket[i] == id){
                   bucket[i] = null;
                   no_elements-=1;
                   for(int j = i; j < bucket_size - 1; j++){
                        bucket[j] = bucket[j+1];
                   }
                   bucket[bucket_size - 1] = null;
                   break;
              }
         }
         return isEmpty();
    }
    
    public void makeNull(){
         for(int i = 0; i < bucket_size; i++){
              bucket[i] = null;
         }
         no_elements = 0;
    }

    public boolean isOverflowed(){
        if(this.no_elements + 1 > this.bucket_size){
             return true;
        }
        return false;
    }

    public boolean isEmpty(){
         if(this.no_elements == 0){
              return true;
         }
         return false;
    }
 }