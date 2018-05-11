import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.RandomAccessFile;

public class SystemCatalogue {
	  InputStreamReader console = new InputStreamReader(System.in);
	  BufferedReader console2 = new BufferedReader(console);
	  public SystemCatalogue() throws IOException{
		  File sysCat = new File("SysCat.txt");
		  RandomAccessFile output =new RandomAccessFile (sysCat, "rw");
		  output.seek(0);
	      boolean dataf = (new File("DataFiles")).mkdir();
	      
	    	  output.writeInt(0);
	     
	      boolean exit=false;
	      boolean firsttime=false;
	      while(!exit){	  
	    	if(firsttime)
	    		System.out.println("What do you want to do?");
	    	System.out.println("\n\n");
	    	System.out.println("DDL Operations\n");
	      	System.out.println("1- Create a record type.");
	      	System.out.println("2- Delete a record type.");
	      	System.out.println("3- List all types.\n");
	      	System.out.println("DML Opreations\n");
	      	System.out.println("4- Create a record.");
	      	System.out.println("5- Delete a record.");
	      	System.out.println("6- Search for a record(by primary key).");
	      	System.out.println("7- List all records of a type.");
	      	System.out.println("8- Exit.");
	      	int num=Integer.parseInt(console2.readLine());
	      	firsttime=true;
	      	if(num==1){
	      		CreateRecordType();
	      	}
	      	else if(num==2){
	      		DeleteRecordType();
	      	}
	      	else if(num==3){
	      		ListAllTypes();
	      	}
	      	else if(num==4){
	      		CreateRecord();
	      	}
	      	else if(num==5){
	      		DeleteRecord();
	      	}
	      	else if(num==6){
	      		SearchRecord();
	      	}
	      	else if(num==7){
	      		ListAllRecords();
	      	}
	      	else if(num==8){
	      		exit=true;
	      		System.out.println("Bye!");
	      	}
	      	else{
	      		System.out.println("Please enter a valid number.");
	      	}
	      	}

	  }
	private void ListAllRecords() throws IOException {
		// TODO Auto-generated method stub
		System.out.println("\nPlease enter type name:");
		String typeName=console2.readLine();
		File f=new File("DataFiles/"+typeName+".txt");
		RandomAccessFile output=new RandomAccessFile(f,"rw");
		output.seek(0);
		int numOfRecords=output.readInt();
		output.seek(4);
		int numOfFields=output.readInt();
		String[] fieldNames=new String[numOfFields];
		for(int i=0;i<numOfFields;i++){
			output.seek(8+10*i);
			String fieldName=output.readUTF();
			fieldNames[i]=fieldName;
		}
		int pcounter=1;
		System.out.println("Searching in page "+ pcounter);
		for(int i=0;i<numOfRecords;i++){
			if(i%32==0&&i!=0){
				System.out.println("Reading page "+pcounter);
			}
			output.seek(8+10*numOfFields+(4*numOfFields+1)*i);
			boolean isDeleted=output.readBoolean();
			output.seek(8+10*numOfFields+(4*numOfFields+1)*i+1);
			for(int j=0;j<numOfFields;j++){
				System.out.print(fieldNames[j]+": ");
				output.seek(8+10*numOfFields+(4*numOfFields+1)*i+1+j*4);
				int fieldVal=output.readInt();
				System.out.print(fieldVal+"   ");
			}
			if(isDeleted)
				System.out.print("Status:Deleted");
			System.out.println();
		}
		
	}
	private void SearchRecord() throws IOException {
		// TODO Auto-generated method stub
		System.out.println("\nPlease enter type name:");
		String typeName=console2.readLine();
		File f=new File("DataFiles/"+typeName+".txt");
		RandomAccessFile output=new RandomAccessFile(f,"rw");
		output.seek(0);
		int numOfRecords=output.readInt();
		output.seek(4);
		int numOfFields=output.readInt();
		System.out.println("\nPlease enter the primary key of the record you want to search:");
		int primaryKey=Integer.parseInt(console2.readLine());
		String[] fieldNames=new String[numOfFields];
		boolean canFind=false;
		for(int i=0;i<numOfFields;i++){
			output.seek(8+10*i);
			String fieldName=output.readUTF();
			fieldNames[i]=fieldName;
		}
		int pcounter=1;
		System.out.println("Searching in page "+ pcounter);
		output.seek(8+10*numOfFields);
		for(int i=0;i<numOfRecords;i++){
			if(i%32==0&&i!=0){
				pcounter++;
				System.out.println("Searching in page "+ pcounter);
			}
			output.seek(8+10*numOfFields+(4*numOfFields+1)*i+1);
			if(output.readInt()==primaryKey){
				canFind=true;
				for(int j=0;j<numOfFields;j++){
					output.seek(8+10*numOfFields+(4*numOfFields+1)*i+1+j*4);
					int s=output.readInt();
					System.out.print(fieldNames[j]+": ");
					System.out.println(s);
					
				}
				output.seek(8+10*numOfFields+(4*numOfFields+1)*i);
				if(output.readBoolean()){
					System.out.println("This record has been deleted");
				}
				canFind=true;
			}
			
		}
		if(!canFind)
			System.out.println("There's no record with the given primary key.");
	}
	private void DeleteRecord() throws IOException {
		// TODO Auto-generated method stub
		System.out.println("\nPlease enter type name:");
		String typeName=console2.readLine();
		File f=new File("DataFiles/"+typeName+".txt");
		RandomAccessFile output =new RandomAccessFile (f, "rw");
		output.seek(0);
		int numOfRecords=output.readInt();
		output.seek(4);
		int numOfFields=output.readInt();
		System.out.println("\nPlease enter the primary key of the record you want to delete:");
		int primaryKey=Integer.parseInt(console2.readLine());
		output.seek(8+10*numOfFields);
		boolean canDelete=false;
		int pcounter=1;
		System.out.println("Reading page "+ pcounter);
		for(int i=0;i<numOfRecords;i++){
			if(i%32==0&&i!=0){
				pcounter++;
				System.out.println("Reading page "+ pcounter);
			}
			output.seek(8+10*numOfFields+(4*numOfFields+1)*i);
			if(!output.readBoolean()){
				//System.out.println("hi im here");
				output.seek(8+10*numOfFields+(4*numOfFields+1)*i+1);
				int fieldVal=output.readInt();
				if(fieldVal==primaryKey){
					output.seek(8+10*numOfFields+(4*numOfFields+1)*i);
					output.writeBoolean(true);
					System.out.println("Record has been deleted.");
					canDelete=true;
					break;
				}
			}
		}
		if(!canDelete)
			System.out.println("There's no record with the given primary key.");

	}
	private void CreateRecord() throws IOException {
		// TODO Auto-generated method stub
		System.out.println("\n Please write the record type.");
		String typeName=console2.readLine();
		File f=new File("DataFiles/"+typeName+".txt");
		RandomAccessFile output =new RandomAccessFile (f, "rw");
		output.seek(0);
		int numOfRecords=output.readInt();
		numOfRecords++;
		output.seek(0);
		output.writeInt(numOfRecords);
		output.seek(4);
		int numOfFields=output.readInt();
		int []fieldValues=new int[numOfFields];
		for(int i=0;i<numOfFields;i++){
			output.seek(8+10*i);
			String fieldName=output.readUTF();
			System.out.println("Please enter "+fieldName+" value:");
			fieldValues[i]=Integer.parseInt(console2.readLine());
		}
		
		output.seek(8+10*numOfFields+((4*numOfFields+1)*(numOfRecords-1)));
		output.writeBoolean(false);
		output.seek(8+10*numOfFields+((4*numOfFields+1)*(numOfRecords-1))+1);
		int pcounter=1;
		System.out.println("Reading page "+ pcounter);
		for(int i=0;i<numOfRecords;i++){
			if(i%32==0&&i!=0){
				pcounter++;
				System.out.println("Reading page "+pcounter);
			}
		}
		for(int i=0;i<numOfFields;i++){
			output.seek(8+10*numOfFields+((4*numOfFields+1)*(numOfRecords-1))+1+4*i);
			output.writeInt(fieldValues[i]);
		}
		
		
	}
	private void ListAllTypes() throws IOException {
		// TODO Auto-generated method stub
		File sysCat = new File("SysCat.txt");
		RandomAccessFile output =new RandomAccessFile (sysCat, "rw");
		output.seek(0);
		int numOfTypes=output.readInt();
		output.seek(4);
		for(int i=0;i<numOfTypes;i++){
			output.seek(4+115*i);
			String typeName=output.readUTF();
			output.seek(1+115*i+14);
			boolean isDeleted=output.readBoolean();
			if(!isDeleted)
				System.out.println(typeName);
		}
	}
	private void DeleteRecordType() throws IOException {
		// TODO Auto-generated method stub
		System.out.println("Please enter a type name.");
		String typeName=console2.readLine();
		System.out.println(typeName);
		File f=new File("DataFiles/"+typeName+".txt");
		f.delete();
		//System.out.println(f.delete());
		File sysCat = new File("SysCat.txt");
		RandomAccessFile output =new RandomAccessFile (sysCat, "rw");
		output.seek(0);
		int numOfTypes=output.readInt();
		output.seek(4);
		for(int i=0;i<numOfTypes;i++){
			output.seek(4+115*i);
			String type=output.readUTF();
			if(type.equals(typeName)){
				output.seek(1+115*i+14);
				output.writeBoolean(true);
			}
			
		}
		System.out.println(typeName+" is deleted.");
	}
	private void CreateRecordType() throws IOException {
		// TODO Auto-generated method stub
		System.out.println("Please enter type name.");
		String typeName=console2.readLine();
		while(alreadyExists(typeName)|typeName.length()>10|typeName.length()==0){
			System.out.println("Please enter a valid type name.");
			typeName=console2.readLine();
		}
		File record=new File("DataFiles/"+typeName+".txt");
		System.out.println("Enter the number of fields.");
		int numOfFields=Integer.parseInt(console2.readLine());
		String[] fieldNames=new String[numOfFields];
		for(int i=0;i<numOfFields;i++){
			System.out.println("field name "+(i+1)+":");
			fieldNames[i]=console2.readLine();
		}
		RandomAccessFile out = new RandomAccessFile(record , "rw");
		out.seek(0);
		out.writeInt(0);
		out.seek(4);
		out.writeInt(numOfFields);
		out.seek(8);
		for(int i=0;i<numOfFields;i++){
			out.seek(8+10*i);
			out.writeUTF(fieldNames[i]);
		}
		
		File sysCat = new File("SysCat.txt");
		RandomAccessFile output =new RandomAccessFile (sysCat, "rw");
		output.seek(0);
		int numOfTypes=output.readInt();
		numOfTypes++;
		output.seek(0);
		output.writeInt(numOfTypes);
		output.seek((numOfTypes - 1) * 115 + 4);
		output.writeUTF(typeName);
		output.seek((numOfTypes - 1) * 115 + 14);
		output.writeInt(numOfFields);
		output.seek((numOfTypes - 1) * 115 + 18);
		output.writeBoolean(false);
		output.seek((numOfTypes - 1) * 115 + 19);
		for(int i=0;i<fieldNames.length;i++){
			output.seek((numOfTypes - 1) * 115 + 19+i*10);
			output.writeUTF(fieldNames[i]);
		}
	}
	
	
	
	
	
	
	
	
	public boolean alreadyExists(String inp){
		File types=new File("DataFiles");
		String[] f=types.list();
		for(int i=0;i<f.length;i++){
			if(f[i].equals(inp+".txt"))
				return true;
		}
		return false;

	}
	  
	  

}
