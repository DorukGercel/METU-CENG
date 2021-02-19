package ceng.ceng351.labdb;
import java.util.ArrayList; 

public class LabDB {
    
    private int global_depth;
    private int bucket_size;
    private ArrayList<Bucket> directory;
    private int directory_size;

    public LabDB(int bucketSize) {
        this.global_depth = 1;
        this.bucket_size = bucketSize;
        this.directory = new ArrayList<Bucket>(2);
        this.directory_size = 2;

        directory.add(new Bucket(bucket_size));
        directory.add(new Bucket(bucket_size));
    }

    public void enter(String studentID) {
        boolean flag = false;
        int index = hashFunction(studentID);
        if(directory.get(index).addElement(studentID)){
            return;
        }
        else{
            while(!flag){
                if(needToExtend(directory.get(index).getLocalDepth())){
                    extendTable();
                    if(splitBuckets(index, studentID)){
                       flag = true; 
                    }
                }
                else{
                    if(splitBuckets(index, studentID)){
                        flag = true;
                    }
                }
            }
        }
    }

    public void leave(String studentID) {
        int index = hashFunction(studentID);
        if(!directory.get(index).deleteElement(studentID)){
            return;
        }
        else{
            while(mergeBuckets(index)){}
            while(needToShrink()){
                shrinkTable();
            }
        }
        
    }

    public String search(String studentID) {
        int index = hashFunction(studentID);
        int size = directory.get(index).getNoElements();
        for(int i = 0; i < size; i++){
            if(directory.get(index).getElement(i).equals(studentID)){
                return suffixMaker(index);   
            }
        }
        return "-1";
    }

    public void printLab() {
        System.out.print("Global depth");
        System.out.print(" : ");
        System.out.print(global_depth + "\n");

        for(int i = 0; i < directory_size; i++){
            System.out.print(suffixMaker(i));
            System.out.print(" : ");
            System.out.print("[Local depth:" + directory.get(i).getLocalDepth()+ "]");
            for(int j = 0; j < bucket_size; j++){
                if(directory.get(i).getElement(j) != null){
                    System.out.print("<");
                    System.out.print(directory.get(i).getElement(j));
                    System.out.print(">");
                }
            }
            System.out.print("\n");
        }
    }
    /******************************************************************************************************************/
    public int hashFunction(String id){
        String no = id.substring(1);
        int studentID = Integer.parseInt(no);
        int mod = directory_size;

        return (studentID % mod);
    }

    public void extendTable(){
        this.global_depth += 1;

        for(int i = 0; i < directory_size; i++){
            directory.add(directory.get(i));
        }
        this.directory_size *= 2;
    }

    public void shrinkTable(){
        this.global_depth -= 1;
        int size = powerOf(2, global_depth);
        
        for(int i = size; i < directory_size; i++){
            directory.remove(size);
        }

        directory_size = size;
    }

    public boolean splitBuckets(int index, String new_element){
        //Set local depth and find the frequency
        boolean flag = true;
        int local_depth = powerOf(2,directory.get(index).getLocalDepth());
        //Both first and second one are set
        index = index % local_depth;
        int tmp_index = index + local_depth;
        
        //Allocate temp array to hold size+1 variables
        String temp[] = new String[bucket_size + 1];
        directory.set(tmp_index, new Bucket(bucket_size, directory.get(index).getLocalDepth()));
        
        //Copy in temp
        for(int i = 0; i < bucket_size; i++){
            temp[i] = directory.get(index).getElement(i);
        }
        temp[bucket_size] = new_element;

        //Link indexes and make clean buckets
        for(int i = 0; i < directory_size; i += local_depth*2){
            directory.get(index + i).makeNull();
            directory.get(tmp_index + i).makeNull();

            directory.set(index + i, directory.get(index));
            directory.set(tmp_index + i, directory.get(index + local_depth));
        }

        //Add elements
        for(int i = 0; i < bucket_size + 1; i++){
            int split_index = hashFunction(temp[i]);
            if(!directory.get(split_index).addElement(temp[i])){
                flag = false;
            }
        }

        //Increment local depth
        directory.get(index).incrementLocalDepth();
        directory.get(tmp_index).incrementLocalDepth();
        return flag;
    }

    //Merge buckets. Output: Need to merge again?
    public boolean mergeBuckets(int index){
        int local_depth;
        if((directory.get(index).getNoElements() == 0) && ((local_depth = directory.get(index).getLocalDepth())>1)){
            int jump_size = powerOf(2, local_depth - 1);
            int new_index_1 = index % jump_size, new_index_2 = new_index_1 + jump_size;
            if(directory.get(new_index_2).getNoElements() == 0){
                new_index_2 = new_index_1;
                new_index_1 += jump_size; 
            }
            for(int i = new_index_1; i < directory_size; i+= 2*jump_size){
                if(directory.get(new_index_1).getLocalDepth() == directory.get(new_index_2).getLocalDepth()){
                    directory.set(new_index_1, directory.get(new_index_2));
                    directory.get(new_index_1).decrementLocalDepth();
                }
                new_index_1 += 2*jump_size;
                new_index_2 += 2*jump_size;
            }
            if((directory.get(index).getNoElements() == 0)){
                return false;
            }
            return true;
        }
        return false;
        
    }

    public boolean needToExtend(int i){
        return (this.global_depth == i);
    }

    public boolean needToShrink(){
        boolean flag = true;
        for(int i = 0; i < directory_size; i++){
            if(this.global_depth == directory.get(i).getLocalDepth()){
                flag = false;
                break;
            }
        }
        return flag;
    }

    public String suffixMaker(int suffix){
        int total_digit = digit_maker(directory_size-1);
        int suffix_digit = digit_maker(suffix);
        String str = "";
        for(int i = 0; i < total_digit - suffix_digit; i++){
            str = str + "0";
        }
        str = str + Integer.toBinaryString(suffix);
        return str;
    }

    public int digit_maker(int number){
        int i = 1;
        int x = number;
        while(x > 1){
            x = x/2;
            i+=1;
        }
        return i;
    }

    public int powerOf(int x, int y){
        int result = x;
        for(int i = 1; i < y; i++){
            result *= x;
        }
        return result;
    }

    
}