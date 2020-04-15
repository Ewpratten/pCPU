/**
   Arduino CPU Project
   by: Evan Pratten
*/

/***********************
   Definitions for I/O
 ***********************/

// ROM Write Input
#define IN_WRITE_1 6
#define IN_WRITE_2 7
#define IN_WRITE_3 8
#define IN_WRITE_4 9

// Input Mode Buttons
#define NIBBLE_SELECT 5
#define ACTION_BUTTON 4
#define MODE_SEL A0

bool last_ab = false;

// LED Outputs
#define OUT_DISP_1 10
#define OUT_DISP_2 16
#define OUT_DISP_3 14
#define OUT_DISP_4 15

// Cycle type
// If false, read inputs
// If true, write outputs
bool cycle_type = false;

// System Mode
#define PGRM 1
#define RUN 2

int mode = PGRM;
bool debug_display = true;

// Memory
#define MEM_SIZE 512
#define MEM_PGRM_START 20

int system_memory[MEM_SIZE];
int mem_ptr = MEM_PGRM_START; // Program memory starts at 0001 1011

// Registers
int system_registers[4];

// Datatypes
#define DATTYPE_I8 128    // 1000 0000
#define DATTYPE_I16 64    // 0100 0000
#define DATTYPE_REG 192   // 1100 0000
#define DATTYPE_MEM8 32   // 0010 0000
#define DATTYPE_MEM16 160 // 1010 0000

// Instructions
#define INS_NOP 0   // 0000 0000
#define INS_MOV 1   // 0000 0001
#define INS_ADD 2   // 0000 0010
#define INS_SUB 3   // 0000 0011
#define INS_MUL 4   // 0000 0100
#define INS_DIV 5   // 0000 0101
#define INS_CALL 6  // 0000 0110
#define INS_JMP 7   // 0000 0111
#define INS_CMP 8   // 0000 1000
#define INS_CMPL 9  // 0000 1001
#define INS_CMPG 10 // 0000 1010
#define INS_NCMP 11 // 0000 1011

// System calls
#define SYSCALL_HALT 128 // 1000 0000
#define SYSCALL_DISP 64  // 0100 0000

typedef struct Variable {
  int type;
  int raw_val;
  int val;
};

Variable processVar() {

  Variable v;

  // Read datatype
  v.type = system_memory[mem_ptr];
  mem_ptr++;

  // If the type is a 2-byte type, handle it
  if (v.type == DATTYPE_I16 || v.type == DATTYPE_MEM16) {

    int a = system_memory[mem_ptr];
    mem_ptr++;

    int b = system_memory[mem_ptr];
    mem_ptr++;

    // Combine bytes
    v.raw_val = a | (b << 8);

  } else {
    v.raw_val = system_memory[mem_ptr];
    mem_ptr++;
  }

  // Handle "remote" values
  if (v.type == DATTYPE_REG) {
    v.val = system_registers[v.raw_val];
  } else if (v.type == DATTYPE_MEM8 || v.type == DATTYPE_MEM16) {
    v.val = system_memory[v.raw_val];
  } else {
    v.val = v.raw_val;
  }

  return v;

}

/* Instructions */
void ins_mov() {

  // Read the dest
  Variable dest = processVar();

  // Read the value
  Variable val = processVar();

  // Determine if the dest is a register or not
  if (dest.type == DATTYPE_REG) {
    system_registers[dest.raw_val] = val.val; 
  } else {
    system_memory[dest.raw_val] = val.val;
  }

}

void ins_add() {

  // Read source A
  Variable a = processVar();

  // Read source b
  Variable b = processVar();

  // Add
  int result = a.val + b.val;

  // Write result to the first register
  system_registers[0] = result;
}

void ins_sub() {

  // Read source A
  Variable a = processVar();

  // Read source b
  Variable b = processVar();

  // Add
  int result = a.val - b.val;

  // Write result to the first register
  system_registers[0] = result;
}

void ins_mul() {

  // Read source A
  Variable a = processVar();

  // Read source b
  Variable b = processVar();

  // Add
  int result = a.val * b.val;

  // Write result to the first register
  system_registers[0] = result;
}

void ins_div() {

  // Read source A
  Variable a = processVar();

  // Read source b
  Variable b = processVar();

  // Add
  int result = a.val / b.val;

  // Write result to the first register
  system_registers[0] = result;
}

void call_disp() {

  // Set the display to read from it's own register
  debug_display = false;

  // Read the stored number
  int sto = system_registers[0];

  // Write the stored number to the display
  system_memory[2] = sto;

}

void ins_call() {

  // Read subroutuine number
  Variable srn = processVar();

  // Handle the appropriate subroutuine
  if (srn.val == SYSCALL_DISP) {
    call_disp();
  }

}

void ins_jmp() {

  // Read the address to jump to
  Variable dest = processVar();

  // Set the pointer to the dest
  mem_ptr = dest.val;
}

void ins_cmp() {

  // Read source A
  Variable a = processVar();

  // Read source b
  Variable b = processVar();

  // If the two are not equal, we skip the next instruction
  if(a.val != b.val){
    mem_ptr++;
  }
}

void ins_cmpl() {

  // Read source A
  Variable a = processVar();

  // Read source b
  Variable b = processVar();

  // If the two are not lt, we skip the next instruction
  if(a.val >= b.val){
    mem_ptr++;
  }
}

void ins_cmpg() {

  // Read source A
  Variable a = processVar();

  // Read source b
  Variable b = processVar();

  // If the two are not gt, we skip the next instruction
  if(a.val <= b.val){
    mem_ptr++;
  }
}

void ins_ncmp() {

  // Read source A
  Variable a = processVar();

  // Read source b
  Variable b = processVar();

  // If the two are equal, we skip the next instruction
  if(a.val == b.val){
    mem_ptr++;
  }
}


/* Main Program */
void setup() {

  // Init I/O
  pinMode(IN_WRITE_1, INPUT);
  pinMode(IN_WRITE_2, INPUT);
  pinMode(IN_WRITE_3, INPUT);
  pinMode(IN_WRITE_4, INPUT);

  pinMode(NIBBLE_SELECT, INPUT);
  pinMode(ACTION_BUTTON, INPUT);
  pinMode(MODE_SEL, INPUT);

  pinMode(OUT_DISP_1, OUTPUT);
  pinMode(OUT_DISP_2, OUTPUT);
  pinMode(OUT_DISP_3, OUTPUT);
  pinMode(OUT_DISP_4, OUTPUT);


  Serial.begin(9600);
}

void handleInput() {

  // Read inputs to memory
  if (digitalRead(NIBBLE_SELECT)) {
    system_memory[1] &= 15;

    // Bit 1
    if ((digitalRead(IN_WRITE_1) != (system_memory[1] & 16)) && digitalRead(IN_WRITE_1)) {
      if (system_memory[0] & 16) {
        system_memory[0] &= 239;
      } else {
        system_memory[0] |= 16;
      }
    }
    system_memory[1] |= ((digitalRead(IN_WRITE_1)) ? 16 : 0);

    // Bit 2
    if ((digitalRead(IN_WRITE_2) != (system_memory[1] & 32)) && digitalRead(IN_WRITE_2)) {
      if (system_memory[0] & 32) {
        system_memory[0] &= 223;
      } else {
        system_memory[0] |= 32;
      }
    }
    system_memory[1] |= ((digitalRead(IN_WRITE_2)) ? 32 : 0);

    // Bit 3
    if ((digitalRead(IN_WRITE_3) != (system_memory[1] & 64)) && digitalRead(IN_WRITE_3)) {
      if (system_memory[0] & 64) {
        system_memory[0] &= 191;
      } else {
        system_memory[0] |= 64;
      }
    }
    system_memory[1] |= ((digitalRead(IN_WRITE_3)) ? 64 : 0);

    // Bit 4
    if ((digitalRead(IN_WRITE_4) != (system_memory[1] & 128)) && digitalRead(IN_WRITE_4)) {
      if (system_memory[0] & 128) {
        system_memory[0] &= 127;
      } else {
        system_memory[0] |= 128;
      }
    }
    system_memory[1] |= ((digitalRead(IN_WRITE_4)) ? 128 : 0);

  } else {
    system_memory[1] &= 240;

    // Bit 1
    if ((digitalRead(IN_WRITE_1) != (system_memory[1] & 1)) && digitalRead(IN_WRITE_1)) {
      if (system_memory[0] & 1) {
        system_memory[0] &= 254;
      } else {
        system_memory[0] |= 1;
      }
    }
    system_memory[1] |= ((digitalRead(IN_WRITE_1)) ? 1 : 0);

    // Bit 2
    if ((digitalRead(IN_WRITE_2) != (system_memory[1] & 2)) && digitalRead(IN_WRITE_2)) {
      if (system_memory[0] & 2) {
        system_memory[0] &= 253;
      } else {
        system_memory[0] |= 2;
      }
    }
    system_memory[1] |= ((digitalRead(IN_WRITE_2)) ? 2 : 0);

    // Bit 3
    if ((digitalRead(IN_WRITE_3) != (system_memory[1] & 4)) && digitalRead(IN_WRITE_3)) {
      if (system_memory[0] & 4) {
        system_memory[0] &= 251;
      } else {
        system_memory[0] |= 4;
      }
    }
    system_memory[1] |= ((digitalRead(IN_WRITE_3)) ? 4 : 0);

    // Bit 4
    if ((digitalRead(IN_WRITE_4) != (system_memory[1] & 8)) && digitalRead(IN_WRITE_4)) {
      if (system_memory[0] & 8) {
        system_memory[0] &= 247;
      } else {
        system_memory[0] |= 8;
      }
    }
    system_memory[1] |= ((digitalRead(IN_WRITE_4)) ? 8 : 0);
  }


  if (mode == PGRM) {

    // Copy inputs to current mem addr
    system_memory[mem_ptr] = system_memory[0];

    // Handle mem addr incr and decr requests
    if (digitalRead(ACTION_BUTTON) != last_ab && digitalRead(ACTION_BUTTON)) {
      if (digitalRead(NIBBLE_SELECT)) {
        mem_ptr = max(0, mem_ptr - 1);
      } else {
        mem_ptr = min(MEM_SIZE - 1, mem_ptr + 1);
      }
      system_memory[0] = system_memory[mem_ptr];
      system_memory[1] = 0;
    }

    last_ab = digitalRead(ACTION_BUTTON);
  }

  Serial.print(mem_ptr, BIN);
  Serial.print(" ");
  Serial.print(system_memory[mem_ptr], BIN);
  Serial.print("\n");

}

void handleOutput() {

  // Read display from memory
  int disp;
  if (debug_display) {
    disp = system_memory[mem_ptr];
  } else {
    disp = system_memory[2];
  }

  // Shift outputs if the shift button is held
  if (digitalRead(NIBBLE_SELECT)) {

    // Write status to each LED
    digitalWrite(OUT_DISP_1, disp & 16);
    digitalWrite(OUT_DISP_2, disp & 32);
    digitalWrite(OUT_DISP_3, disp & 64);
    digitalWrite(OUT_DISP_4, disp & 128);
  } else {

    // Write status to each LED
    digitalWrite(OUT_DISP_1, disp & 1);
    digitalWrite(OUT_DISP_2, disp & 2);
    digitalWrite(OUT_DISP_3, disp & 4);
    digitalWrite(OUT_DISP_4, disp & 8);
  }

}

void handleLogic() {

  // Read mode type
  if (digitalRead(MODE_SEL)) {
    if (mode == RUN) {
      // Reset the pointer
      mem_ptr = MEM_PGRM_START;

      // Enable debug display
      debug_display = true;
    }
    mode = PGRM;
  } else {
    if (mode != RUN) {
      // Reset the pointer
      mem_ptr = MEM_PGRM_START;

      // Enable debug display
      debug_display = true;
    }
    mode = RUN;
  }

  if (mode == RUN) {

    // Read the current instruction
    int ins = system_memory[mem_ptr];
    mem_ptr++;

    // Handle instruction
    if ( ins == INS_MOV ) {
      ins_mov();
    } else if ( ins == INS_ADD ) {
      ins_add();
    } else if ( ins == INS_SUB ) {
      ins_sub();
    } else if ( ins == INS_MUL ) {
      ins_mul();
    } else if ( ins == INS_DIV ) {
      ins_div();
    } else if ( ins == INS_CALL ) {
      ins_call();
    } else if ( ins == INS_JMP ) {
      ins_jmp();
    } else if ( ins == INS_CMP ) {
      ins_cmp();
    } else if ( ins == INS_CMPL ) {
      ins_cmpl();
    } else if ( ins == INS_CMPG ) {
      ins_cmpg();
    } else if ( ins == INS_NCMP ) {
      ins_ncmp();
    }

  }

}

void loop() {

  // Handle read vs write
  if (cycle_type) {
    handleInput();
  } else {
    handleOutput();
  }

  // Handle logic
  handleLogic();

  cycle_type = !cycle_type;

}