/* 
Members: Christian Barajas, Cody Cosby, Kevin Nguyen, Wanqi Chin
Account: masc0833
Team:    Alabama       
Assignment: prog2
FileName: sixce_asm.cpp
Class: CS530, Spring 2016
*/

#include "sicxe_asm.h"

using namespace std;

// main method: reads filename from command line
int main(int argc, char* argv[]){
	if(argc != 2){ //only two command arguments are valid
		cout << "Error, you must provide at least and no more than 1 filename to be processed." << endl;
		exit(1);
	}
	
	sicxe_asm *asmprog = new sicxe_asm();
	asmprog->assemble(argv[1]);

	return 0;
}

//constructor
sicxe_asm::sicxe_asm(){
    base= -1;
    current_line=0;
    locctr= 0;
    symbtable = new symtab();
    optable = new opcodetab();
}

//deconstructor
sicxe_asm:: ~sicxe_asm(){}

void sicxe_asm::assemble(string filename){ //opens file, parses it, and outputs it to a .lis file
    try{
        toParse(filename);
    }
    catch (file_parse_exception e){
        cout<< e.getMessage()<< endl;
        return;
    }
    first_pass();
    create_listing_file(filename);
}
void sicxe_asm::toParse(string s){ //creates new file_parser
    fparse= new file_parser(s);
    fparse-> read_file();
}












//``````````` PASS 1 & 2``````````````````````````````````````
// ````````````````````````````````````````````````````````````````````````````



bool sicxe_asm::first_pass(){ //gets label, opcode, operand stores it into struct to_print:: list, they are then pushed onto vector list_vector for printing
    try{
        do{
            current_line++;
            to_print list;
            list.line = current_line;
            list.address = int_to_hex(locctr, 6);
	    list.label = fparse->get_token(current_line-1,0); //lines start at 1
   	    if(list.label != "" && (to_upper(fparse->get_token(current_line-1,1)) != "EQU")) 
      	        symbtable->insert_symbol(list.label,int_to_hex(locctr, 6),current_line);
	    list.opcode = fparse->get_token(current_line-1,1);
            list.operand = fparse->get_token(current_line-1,2);
	    if(list.opcode != "")
            	locctr = next_location(list);
            list_vector.push_back(list);
        } 
        while (has_next_line());
     }
     catch (opcode_error_exception e){
        cout<< e.getMessage()<< "Error at: " << current_line<< endl;
        return false;
    }
    return true;
}
bool sicxe_asm::second_pass(){
    test_operand= to_upper(operand);
    test_opcode= to_upper(opcode);
    
    for(int i=0; i<list_vector.size();i++)
    {
        pc = list_vector.at(i+1).address;
    }
}






//````````````````````````````````````````````````````````````````````````````








bool sicxe_asm::has_next_line(){ //terminates if has_next_line is false
    try{
        fparse->get_token(current_line, 0);
    }
    catch(file_parse_exception e){
        return false;
    }
    return true;
    
}

//gets next location for locctr
int sicxe_asm::next_location(to_print &t){
    string opcode = to_upper(t.opcode);
    string operand = t.operand;
    if(opcode == "START"){
		locctr = type_of_operand(operand); //parse operand
		program = fparse->get_token(current_line-1,0);
		return locctr;
	}
	else if (opcode == "END"){ 
		if(operand != program && operand != "")
			throw opcode_error_exception("END operand doesn't match program name.");
		return -1;
	}
	else if (opcode == "BYTE"){ 
		string byte_operand= to_upper(fparse->get_token(current_line-1, 2));
		if (byte_operand[0]== 'C'){
		    return locctr + (byte_operand.length()-3);
		}
		else if(byte_operand[0]= 'X'){
		    if((byte_operand.length()-3)% 2 ==0)
		        return locctr + ((byte_operand.length()-3)/2);
		    else
		        throw opcode_error_exception ("Hex literals must have even number of characters. Error on " + current_line);
		        return locctr;
		}
	}
	else if (opcode == "WORD"){ 
		return locctr+3;
	}
	else if (opcode == "RESB"){ 
		return locctr+(string_to_int(operand)); 
	}
	else if (opcode == "RESW"){ 
		return locctr+(string_to_int(operand)*3); 
	}
	else if (opcode == "BASE"){
		base = type_of_operand(operand);
		return locctr;	
	}
	else if (opcode == "NOBASE"){
		base = -1;
		return locctr;
	}
	else if (opcode == "EQU"){
		try{
			symbtable->insert_symbol(fparse->get_token(current_line-1,0),int_to_hex(type_of_operand(operand),6),current_line);
			return locctr;
		}
		catch(symtab_exception e){
			cout << e.getMessage() << endl;
		}						
	}
    else{ // opcode is not an assembler directive
        if(opcode == ""){
            return locctr;
        }
        else{
            try{
                return locctr+optable->get_instruction_size(opcode);
                
            }
            catch(opcode_error_exception e){
              cout << e.getMessage() << endl;  
            }
        }
    }
    return locctr;
}
// formats printing into .lis output file
void sicxe_asm::create_listing_file(string file){

	string listing = change_filename(file);
	ofstream listfile(listing.c_str());
	
	listfile << setw(5) << setfill(' ') << "Line#";
	listfile << setw(10) << setfill(' ') << "Address";
	listfile << setw(10) << setfill(' ') << "Label";
	listfile << setw(10) << setfill(' ') << "Opcode";
	listfile << setw(13) << setfill(' ') << "Operand" << endl;
	listfile << setw(5) <<"=====" << setw(10) << "=======" << setw(10) <<"=====" << setw(10)<< "======" << setw(13) << "=======" << endl;
	
	for(unsigned int i = 0;i < list_vector.size()-1;i++){
		listfile << setw(5) << setfill(' ') << list_vector.at(i).line;
		listfile << setw(10) << setfill(' ') << list_vector.at(i).address;
		listfile << setw(10) << setfill(' ') << list_vector.at(i).label;
		listfile << setw(10) << setfill(' ') << list_vector.at(i).opcode;
		listfile << setw(13) << setfill(' ') << list_vector.at(i).operand << endl;

	}
}
//adds .lis to file name
string sicxe_asm::change_filename(string file){
	string f;
	
	for(unsigned int i = 0; i < file.size();i++){
		if(file[i] == '.'){ 
			break;
		}
		
		f += file[i];
	}
	
	f += ".lis";
	return f;
}












//``````````````````````````````````````````````````````````````
//```````````` Conversion Methods```````````````````````````````
//``````````````````````````````````````````````````````````````







//converts strings to uppercase
string sicxe_asm::to_upper(string s){
    for(unsigned int i=0; i<s.length();i++){
        s[i]= toupper(s[i]);
    }
    return s;
}

//converts strings to int values
int sicxe_asm::string_to_int(string s){
    istringstream convert(s);
    int val;
    convert >> val;
    return val;
}
// converts hex values to int
int sicxe_asm::hex_to_int(string s){
    int val;
    sscanf(s.c_str(),"%x",&val);
    return val;
}
//converts int values to string
string sicxe_asm::int_to_string (int val){
    stringstream convert;
    string s;
    convert << val;
    convert >> s;
    return s;
}

//converts decimal values to int
int sicxe_asm::dec_to_int(string s){
    int val;
    sscanf(s.c_str(),"%d",&val);
    return val;
}
// converts int values to hex
string sicxe_asm::int_to_hex(int val, int length){
    stringstream convert;
    convert << setw(length) << setfill('0') << hex << val;
    return to_upper(convert.str());
}

string sicxe_asm::hex_to_binary(string hex){
    string hex_list = "0123456789ABCDEF";
    string bin_list[] = {"0000","0001", "0010", "0011", "0100", "0101", "0110", "0111", "1000", "1001", "1010", "1011", "1100", "1101", "1110", "1111"};
    string bin;
    for(unsigned int i=0; i<m_hex_number.size(); i++)
    {
        for(unsigned int j=0; j<17; j++)
        {
            if(m_hex_number[i]==hex_list[j])
            {
                bin += bin_list[j];
            }
        }
    }
    return bin;
}

string sicxe_asm::binary_to_hex(string m_binary_number)
{
    string hex_list = "0123456789ABCDEF";
    string binary_number = m_binary_number;
    int mod = binary_number.size() % 4;
    string hex = "";
    
    if(mod!=0)
    {
        mod = 4-mod;
        for(int i=mod; i>0; i--)
        {
            binary_number.insert(0,"0");
        }
    }
    
    unsigned int i=0;
    while(i<binary_number.size())
    {
        int hex_list_index = 0;
        for(int j=3; j>=0; j--)
        {
            if(binary_number[i]=='1')
            {
                switch(j)
                {
                    case 3:
                        hex_list_index += 8;
                        break;
                    case 2:
                        hex_list_index += 4;
                        break;
                    case 1:
                        hex_list_index += 2;
                        break;
                    case 0:
                        hex_list_index += 1;
                        break;
                }
            }
            i++;
        }
        hex += (hex_list[hex_list_index]);
    }
    return hex;
}

string sicxe_asm::bool_to_string(bool x){
	if(x) return "1";
	return "0";
}

















//``````````````````````````````````````````````````





//handles different types of operands 
int sicxe_asm::type_of_operand(string s){
    if (s[0] == '#'){
        s = s.substr(1);
        
        if (isalpha(s[0])){
            return hex_to_int(symbtable->get_value(s));
        }
        else if (s[0] == '$'){
            return type_of_operand(s);
        }
        return dec_to_int(s);
    }
    else if (s[0] == '$'){ 
        s = s.substr(1);
        return hex_to_int(s);
    }
    else if (isalpha(s[0])){ 
        return hex_to_int(symbtable->get_value(s));
    }
    else{
        return dec_to_int(s); //decimal value not hex
    }
    return 0;
}
//PASS TWO METHODS
bool sicxe_asm::is_after_end()
{
    while(has_next_line())
    {
        for(int i=0; i<3; i++)
        {
            if(parse->get_token(current_line-1,i) != "")
            {
                return true;
            }
        }
    }
}

bool sixce_asm::is_forward_reference(to_print m_list)
{
    if(m_list.label == "" || m_list.label == "prog" || m_list.label =="PROG")
    {
        return true;
    }
    
    if(isalpha((symbtable->get_value(to_upper(m_list.label))).at(0)))
    {
        if(m_list.opcode != "EQU")
        {
            return false;
        }
        if(!val_check(m,value)) return false;
        return true;
    }
}
    
bool sicxe_asm::val_check (to_print m_list, string val){
        try{
            symbtable->set_val(to_upper(m.list.label), symbtable->get_value(to_upper(m_list.operand)));
        }
        catch (symtab_exception e)
            cout<< "Label does not exist at line: " << m_list.line<< endl;
            return false;
    }
    string value= symtable-> get_value(to_upper(m_list.label));
    
    if(isalpha(value.at(0))){
        val_check(m_list,value);
    }
    return true;
}






//````````````````````````````````````````````````````````````````
//``````````````````````BIT FLAGGING``````````````````````````````` 
//`````````````````````````````````````````````````````````````````




//extended bit
void sicxe_asm::extended_bit(to_print &list){
    if(temp_opcode[0]== '+'){
        list.bits.e=1;
         if(list.bits.p==1)
            list.bits.p=0;
        if(list.bits.b==1)
            list.bits.b=0;
    }
    else
        list.bits.e=0;
}
//indirect bit
void sicxe_asm:: indirect_bit(to_print &list){
    if(test_operand[0] == '@')
        list.bits.n=1;
    else
        list.bits.n=0;
}
//immediate bit
void sicxe_asm:: immediate_bit(to_print &list){
    if(test_operand[0]=='#')
        list.bits.i=1;
    else
        list.bits.i=0;
}
//check x bit
void sicxe_asm:: x_bit(to_print &list){
    operand_length= test_operand.length();
    if(((test_operand[0]=='X'))&& (test_operand[1]== ','))|| ((test_operand[operand_length -2]==',')&&(test_operand[operand_length-1]=='X')))
        list.bits.x=1;
    else
        list.bits.x=0;
}

void sicxe_asm::base_bit( to_print &list){
    list.bits.b=1;
    list.bits.p=0;
}

void sicxe_asm::pc_bit(to_print &list){
    //checks for extraneous operands (x register or indirect)
    int i=0;
    while( i<test_operand.length){
        if((test_operand[0]=='X')&& (test_operand[1]==',')){
            test_operand=test_operand.substr(2); 
            break;
            
        }
        if(test_operand[i]==',')
            test_operand=test_operand.substr(0,i);
    }
    //if indirect mode, operand will skip the indirect symbol
    if(test_operand[0]=='@')
        test_operand=test_operand.substr(1);
    //flags p bit if valid label and if operand is an address
    if((symbtable->contains_symbol(test_operand))||(test_operand[0]=='$'))
        list.bits.p=1;
    else
        list.bits.p=0;
    
}
bool sicxe_asm::machine_assemble(to_print &list, bool &pc_mode){
    indirect_bit(list);
    immediate_bit(list);
    x_bit(list);
    pc_bit(list);
    extended_bit(list);
    
    if((list.bits.i==0)&& (list.bits.n==0){
        list.bits.i=1;
        list.bits.n=1;
    }
    try{
        combine_machine(list);
    }
    catch(opcode_error_exception e){
    cout<< e.getMessage()<< endl;
    return false;
    }
    
    get_address_code(list, pc_mode);
    
    return true;
}

void sicxe_asm::get_address_code(to_print &list, bool &pc_mode){
        
    
    int size = -1;
    string m_operand = to_upper(list.operand);
    string opcode = to_upper(list.opcode);
    stringstream m_stream1, m_stream2;
    string m_address, m_displacement;
    bool is_directive;
    
    if(size == -1)
    {
        cout << "Size never changed" << endl;
    }
	
	for(unsigned int i = 0; i < sizeof(directives);i++){
		if(directives[i] == opcode ){ 
			is_directive = true;
		}
	}
	
	if(is_directive || opcode == "") return;
	
	try{
		format = optable->get_instruction_size(opcode);
	}
	catch(opcode_error_exception e){
		cout << "Line " << list.line << "error: " << e.getMessage() << endl;
		size = -1;
	}
	
	// format 1
	if(size == 1){
		list.machine_code = optable->machine_assemble(opcode);
	}
	
	// format 2
	else if(size == 2){
		int comma_loc;
		string first_reg,sec_reg;
		bool no_sec_reg = false;
		string machine_code = optable->machine_assemble(opcode);
		list.machine = machine_code;
		
		for(unsigned int i=0; i<(sizeof(registers)/sizeof(*registers)); i++){
			regs[registers[i]] = reg_values[i];
		}
		if(isalpha(operand[0])){
			for(int i=0; i<operand.length();i++){
				if(operand.length() == i+1){
					no_sec_reg = true;
					first_reg = first_reg + operand[i];
					break;
				}
				
				if(operand[i] == ','){
					comma_loc = i;
					break;
				}
				first_reg = first_reg + operand[i];
			}
		}
		
		else if(isdigit(operand[0])){
		    for(unsigned int i=0; i<operand.length();i++){
		    	if(operand[i] == ','){
					comma_loc = i;
					break;
		    	}
			    first_reg += operand[i];	
			
		    	if(operand.length() == i+1){
		    		no_sec_reg = true;
					first_reg += operand[i];
					break;
		    	}
		    	
		    	if(operand[i] == ',') {
		    		comma_loc = i;
		    		break;
		    	}
		    	
		    	first_reg = first_reg + operand[i];
			}
			
			first_reg = (int_to_hex(string_to_int(first_reg)),1);
			
			if(no_sec_reg) list.machine += "0";
			else{
				sec_reg = operand.substr(comma_loc+1)
				if(isalpha(sec_reg[0])){
					register_iter = regs.find(sec_reg);
					list.machine = list.machine + (register_iter -> <second>);
				}
				
				else if(isdigit(sec_reg[0]))
					list.machine = list.machine + sec_reg;
			}
		}
		
		if(!no_sec_reg) sec_reg = operand.substr(comma_loc+1);
		
		register_iter = regs.find(first_reg);
		list.machine = list.machine + (register_iter -> <second>);
		
		if(no_sec_reg) list.machine = list.machine + "0";
		else{
			if(isalpha(sec_reg[0])){
				register_iter = regs.find(reg2);
				list.machine = list.machine + (register_iter -> <second>);
			}
			
			else if(isdigit(sec_reg[0]))
				list.machine = list.machine + sec_reg;
		}
	}
    
    
    
    
    
    

    //format 3
    else if(size == 3)
    {
        string hexadecimal, start;
        int number;
        
        if(to_upper(list.opcode) == "RSUB")
        {
            list.machine += "000";
        }
        
        if(m_operand[0] == '#')
        {
            if(isdigit(m_operand[1]))
            {
                number = dec_to_int(m_operand.substr(1));
                hexadecimal = int_to_hex(number, 3);
                m_stream1 << setw(3) << setfill('0') << hexadecimal;
                list.machine += m_stream1.str();
            }
            
            
            if(m_operand[1] == '$')
            {
                hexadecimal = m_operand.substr(2);
                m_stream1 << setw(3) << setfill("0") << hexadecimal;
                list.machine += m_stream1.str();
            }
        }
        
        for(int i=0; i<m_operand.length(); i++)
        {
            if(m_operand[0] == 'X' && m_operand[1] == ',')
            {
                m_operand = m_operand.substr(2);
                break;
            }
            
            if(m_operand[i] == ',')
            {
                m_operand = m_operand.substr(0,i);
            }
        }
        
        if(is_label(m_operand))
        {
            //TODO write get_pc_displacement()
            m_displacement = get_pc_displacement(list.operand, list.address);
            
            if(m_displacement == "BASE")
            {
                //TODO write check_base_bit()
                base_bit(list);
                list.machine = "";
                //TODO write combine_machine()
                combine_machine(list);
                //TODO write get_base_relative()
                string base_displacement = get_base_relative(list);
                list.machine += m_displacement;
            }
            else
            {
                list.machine == m_displacement;
            }
        }
        else
        {
            //TODO write operand_type()
            m_stream2 << int_to_hex(operand_type(list.operand), 3);
            m_address = m_stream2.str();
        }
    }
    
    //format 4
    else if(size == 4)
    {
        if(to_upper(list.opcode) == "+RSUB")
        {
            list.machine += "00000";
        }
        
        if(list.operand != "")
        {
            if(m_operand[0]!='#' && m_operand[0]!='@' && m_operand[0]!='$')
            {
                for(int i=0; i<m_operand.length(); i++)
                {
                    if(m_operand[0]=='X' && m_operand[1]==',')
                    {
                        m_operand = m_operand.substr(2);
                        break;
                    }
                    
                    if(m_operand[i] == ',')
                    {
                        m_operand = m_operand.substr(0,i);
                    }
                }
                
                m_address = symbtable->get_value(m_operand);
                m_address = address.substr(1);
            }
            else
            {
                if(m_operand[0] == '$')
                {
                    m_address = m_operand.substr(1);
                }
                if(m_operand[0]=='#' || m_operand[0]=='@')
                {
                    m_operand = m_operand.substr(1);
                    if(isalpha(m_operand[0]))
                    {
                        m_address = symbtable->get_value(m_operand);
                    }
                    
                    if(isdigit(m_operand[0]))
                    {
                        int m_int = string_to_int(m_operand.substr(0));
                        m_address = int_to_hex(m_int, 5);
                    }
                    
                    if(m_operand[0] == '$')
                    {
                        m_operand = m_operand.substr(1);
                        if(isalpha(m_operand[0]))
                        {
                            m_address = symbtable->get_value(m_operand);
                        }
                        
                        if(isdigit(m_operand[0]))
                        {
                            m_address = m_operand.substr(0);
                        }
                    }
                }
                if(m_address.length() == 6)
                {
                    m_address = m_address.substr(1);
                }
            }
            list.machine += m_address;
        }
    }
}

void sicxe_asm::combine_machine(to_print &list)
{
    string machine_part1, machine_part2, machine_part3, machine_opcode;
    string m_code = to_upper(list.opcode);
    
    if(m_code=="" || is_asm_directive(m_code))
    {
        return;
    }
    
    
    machine_opcode = optable->machine_assemble(m_code);
    
    machine_part1 = hex_to_bin(machine_opcode.substr(0,1));
    
    string digit2 = hex_to_bin(machine_opcode.substring(1,1));
    machine_part2 += digit2.substr(0,2);
    
    machine_part2 += int_to_string(list.flags.n);
    machine_part2 += int_to_string(list.flags.i);
    
    machine_part3 += int_to_string(list.flags.x);
    machine_part3 += int_to_string(list.flags.b);
    machine_part3 += int_to_string(list.flags.p);
    machine_part3 += int_to_string(list.flags.e);
    
    list.machine = machine_part1 + machine_part2 + machine_part3;
    
    list.machine = bin_to_hex(list.machine);
}


















































// Program counter displacement... target address-(source+3) 
// Target = operand .... address = refers to address of line
string sicxe_asm::find_pc_displacement(string target, string address){
    string target_address; 
    target = to_upper(target);
    
    try{
        // if destination begins with '@' or '#' then
        // substring it 
        if(target[0]=='@' || target[0]=='#'){
            if(target[1]=='$'){
                target = target.substr(2);
            }
            else{
                target = target.substr(1);
            }
        }
        // if destination begins with '$' then substring it 
        // and assign it to target_address to be used for displacement
        if(target[0]=='$'){
            target_address = target.substr(1);
        }
        
        for(unsigned int i=0; i<target.length(); i++){
            if(target[i] == ','){
                target = target.substr(0,i);
                break;
            }
            if(target[0]=='X' && target[1]==','){
                target = target.substr(2);
                break;
            }
        }
        if(isalpha(target[0])){
            target_address = symtable->get_value(target);
        }
        int destination = hex_to_int(target_address);
        int source = hex_to_int(address);
        int displacement = (destination - (source+3));
        int pc_displacement = int_to_hex(displacement,6);
        
        if(pc_displacement.length() > 6){
            pc_displacement = pc_displacement.substr(pc_displacement.length()-6);
        }
        pc_displacement = pc_displacement.substr(3);
        
        if(reference_overflow(displacement,source,hex_to_bin(pc_displacement))){
            return "BASE";
        }
        return pc_displacement;
    }
    catch(symtab_exception error){
        cout << error.getMessage() << endl;
    }
    return "";
    
}

bool sicxe_asm::label(string s){
	if(s[0] == '@'){
		s = s.substr(1);
		return label(s);
	}
	if(s[0] == '#'){
		s = s.substr(1);
		return label(s);
	}
	if(s[0] == '$'){
		s = s.substr(1);
		return label(s);
	}
	return symtab->contains_symbol(to_upper(s));
}

bool sicxe_asm::reference_overflow(int target, int address, string displacement){
    return (target > address && displacement[0] == '1');
}

string sicxe_asm::find_base_relative(to_print &list){
    string oper = to_upper(list.operand);
    string targ_address;dis
    string base_displacement;
    int base_address = targ = displacement = 0;
    
    try{
        if(oper[0] == '@' || oper[0] == '#'){
            if(oper[1] == '$'){
                oper = oper.substr(2);
            }
            oper = oper.substr(1);
        }
        
        if(oper[0] == '$'){
            targ_address = oper.substr(1);
        }
        
        for(unsigned int i=0; i<oper.length(); i++){
            if(oper[i] == ','){
                oper = oper.substr(0,i);
                break;
            }
            if(oper[0]=='X' && oper[1]==','){
                oper = oper.substr(2);
                break;
            }
        }
        
        if(isalpha(oper[0]))
			targ_address = symtable->get_value(oper);
					
		//change the destination address and the base address to ints
		//for arithmetic			
		targ = hex_to_int(targ_address);
		
		//base is a global variable which is used to find the base
		//address (it is immediately changed from hex to an int)
		base_address = hex_to_int(symtable->get_value(base));
		
		//the displacement is the destination address - base address
		displacement = targ - base_address;
		
		//change the displacement from an int back to the hex 
		//(6 chars for address)
		base_displacement = int_to_hex(displacement,6);
		
		//use only the last 3 chars for the displacement
		base_displacement = base_displacement.substr(3);
		
		//return the base displacement to be used for the second
		//half of the machine code		
		return base_displacement;
	
    }
    
    catch(symtab_exception error){
        cout << "Error line: " << list.line << ". Reference overflow. Use a base reference" << endl;
    }
    return "";
}










int sicxe_asm::asm_directive(string token,string next_token){
	next_token = to_uppercase(next_token);
	
	if(token == "START"){
		//sets the location counter
		location_counter = operand_type(next_token);
		
		//gets the program name
		prog_name = parser->get_token(current_line-1,current_token-3);
	}
	else if (token == "END"){ 
		if(next_token != to_uppercase(prog_name) && next_token != "")
			throw opcode_error_exception("END operand must be program name.");
		return -1;
	}
	else if (token == "BYTE"){ 
		return byte_directive(next_token); 
	}
	else if (token == "WORD"){ 
		return 3;
	}
	else if (token == "RESB"){ 
		return operand_type(next_token); 
	}
	else if (token == "RESW"){ 
		return operand_type(next_token) * 3; 
	}
	else if (token == "BASE"){
		try{
			string t = next_token;
			if(is_label(t))
				base = t;
			else
				base = equ_directive(t);
		}
		catch(symtab_exception e){
			cout << e.getMessage() << endl;
			//cout << base << endl;
		}
			
		return 0;	
	}
	else if (token == "NOBASE"){
		base = "0";
		return string_to_int(base);
	}
	else if (token == "EQU"){
		try{
			//sets the value of the label to the next token as opposed to the 
			//address of the current line
			symbol_table->set_value(to_uppercase(parser->get_token(current_line-1,current_token-3)),equ_directive(next_token));
		}
		catch(symtab_exception e){
			cout << e.getMessage() << endl;
		}
	}						
	
	return 0;
}




string sicxe_asm::equ_directive(string s){
	int tmp = 0;
	//$ mean HEX in this architecture, handle for this
	if (s[0] == '$'){
		s = s.substr(1);
		return s;
	}
	
	//# is an immediate value, handle for this
	else if (s[0] == '#'){
		s = s.substr(1);
		
		if (isalpha(s[0])){
			try{
				return symbol_table->get_value(s);
			}
			catch(symtab_exception e){
				return s;
			}
		}
		
		//there can be a $ after the #
		else if(s[0] == '$'){
			return equ_directive(s);
		}
		tmp = dec_to_int(s);
		return int_to_hex(tmp,6);
	}
	
	else if (s[0] == '@'){
		s = s.substr(1);
		try{
		return symbol_table->get_value(s);
		}
		catch(symtab_exception e){
			return s;
		}
	}
	
	//check for a label in the operand field and get its value
	else if (isalpha(s[0])){
		try{
		return symbol_table->get_value(s);
		}
		catch(symtab_exception e){
			return s;
		}
	}
	else{
		tmp = dec_to_int(s);
		int_to_hex(tmp,6);
	}
	return s;
}

int sicxe_asm::byte_directive(string s){
	if(s[0] == 'X'){
		if(((s.size() - 3) % 2)	!= 0){
			throw opcode_error_exception("Hexadecimal literal " + s + " length must be even.");
		}
		return ( (int)s.size() - 3)/2;
	}
	else if(s[0] == 'C'){
		return (int)s.size()-3;	
	}
	else{
		return (int)s.size();	
	}
	return -1;
}



bool sicxe_asm::is_asm_directive(string s){
	s = to_upper(s);
	
	//loop through the list of directives to find the string in question
	for(unsigned int i = 0; i < sizeof(asm_directives);i++){
		if(asm_directives[i].compare(s) == 0){ 
			return true;
		}
	}
	return false;
}



string sicxe_asm::next_token(){
	try{
		//returns the token following the one currently in question
		//example: the operand after the opcode
		string tmp = parser->get_token(current_line - 1,current_token);
		current_token++;
		return tmp;
	}
	catch(file_parse_exception e){
		//This function should not be called more than 4 times
		return "";
	}
}

bool sicxe_asm::next_line(){
	try{
		//checks for stored tokens to see if there is a next line
		current_token = 0;
		current_line++;
		parser->get_token(current_line-1,current_token);
	}
	catch(file_parse_exception e){
		return false;
	}
	return true;
}

void sicxe_asm::starting_token(string t,list_record &r){
	
	r.line = current_line;
	r.address = int_to_hex(location_counter,6);
	r.label = t;
	
	//make opcode uppercase after getting that token
	//label in question has its value set as long as the opcode following is not EQU 
	if(t != "" && (to_uppercase(parser->get_token(current_line-1,current_token+1)) != "EQU")) {
		symbol_table->set_value(to_uppercase(t),int_to_hex(location_counter,6));
		//cout << "hello" << endl;
	}
}

bool sicxe_asm::middle_token(string t,list_record &r){
	
	
	r.opcode = t;
	t = to_uppercase(t);
	
	//check to see if token is in the list of asm directives
	if(is_asm_directive(t)){
		string next = next_token();
		
		//make sure token is all uppercase and check for the start of the program
		//if there is code before the start of the program that is not the a
		//comment or blank line throw an error
		if (to_uppercase(t) == "START" && location_counter != 0)
			throw opcode_error_exception("Only comments and blank lines may be before START directive.");
		
		//handle the asm directive
		int i = asm_directive(t,next);
		
		if(i == -1){ 
			r.operand = next;
			return false; 
		}
		
		//move forward in the program and store the operand
		location_counter += i;
		r.operand = next;
		return true;
	}
	
	else{
		//move the location counter forward by the opcode's instruction size
		//this will be used for the address
		if(t != ""){
			location_counter += opcode_table->get_instruction_size(t);
		}
			
		r.operand = next_token();
	}
	
	return true;
}






//````````````````````
int sicxe_asm::operand_type(string m_string)
{
    string m_operand_type;
    m_string = to_upper(m_string);
    
    switch (m_string[0])
    {
        case '$':
            m_string = m_string.substr(1);
            return hex_to_int(m_string);
            break;
            
        case '#':
            m_string = m_string.substr(1);
            if(isalpha(m_string[0]))
            {
                try
                {
                    m_operand_type = symbtable->get_value(m_string);
                    
                    if(is_label(m_operand_type))
                    {
                        return 0;
                    }
                    else
                    {
                        return string_to_int(m_operand_type);
                    }
                }
                catch(symtab_exception excep)
                {
                    return 0;
                }
            }
            else if(m_string[0]=='$')
            {
                return operand_type(m_string);
            }
            
            return dec_to_int(m_string);
    
        case '@':
            m_string = m_string.substr(1);
            try
            {
               m_operand_type = symbtable->get_value(m_string);
                        
                if(is_label(m_operand_type))
                {
                    return 0;
                }
                else
                {
                    return string_to_int(m_operand_type);
                }
            }
            catch(symtab_exception excep)
            {
                return 0;
            }
    }
    
    if(isalpha(m_string[0]))
    {
        try
        {
            m_operand_type = symbtable->get_value(m_string);
                    
            if(is_label(m_operand_type))
            {
                return 0;
            }
            else
            {
                return string_to_int(m_operand_type);
            }
        }
        catch(symtab_exception excep)
        {
            return 0;
        }
    }
    else
    {
        return dec_to_int(m_string);
    }
    return 0;
}
