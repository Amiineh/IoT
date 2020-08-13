
const int HEIGHT = 12;
const int WIDTH = 10;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}


int rowToCol(int idx){
  int col = idx % WIDTH;
  int row = idx / WIDTH;
  return (col + 1) * HEIGHT - row - 1;
}
