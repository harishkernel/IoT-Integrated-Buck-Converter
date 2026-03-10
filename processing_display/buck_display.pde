import processing.serial.*;
Serial myPort;
String data = "";
String vin = "", vout = "", iout = "", potStatus = "";

// Background image (optional)

PImage bg;
void setup() {
    fullScreen();
    background(0);
    textSize(40);
    fill(0, 255, 0);
    // Auto-detect ports
    println(Serial.list());
    myPort = new Serial(this, "COM6", 9600); // replace with Arduino port
}

void draw() {
    background(0); // default black background
    // If you have a background image
    // if (bg != null) image(bg, 0, 0, width, height);
    // Header
    fill(0, 200, 255);
    textSize(60);
    textAlign(CENTER);
    text("Embedded System Design Laboratory", width/2, 80);
    textSize(50);
    text("Buck Converter", width/2, 150);
    // Box dimensions
    float boxW = 600;
    float boxH = 120;
    float gap = 40;
    // Left column
    drawBox(100, 250, boxW, boxH, "Input Voltage (Vin): " + vin + " V");
    drawBox(100, 250 + boxH + gap, boxW, boxH, "Output Voltage (Vout): " + vout + " V");
    // Right column
    drawBox(width - boxW - 100, 250, boxW, boxH, "Output Current (Iout): " + iout + " A");
    drawBox(width - boxW - 100, 250 + boxH + gap, boxW, boxH, "Potentiometer: " + potStatus);
    // Read serial data
    while (myPort.available() > 0) {
        data = myPort.readStringUntil(&apos;\n&apos;);
        if (data != null) {
            data = trim(data);
            parseData(data);
        }
    }
}

// Draw big labeled box
void drawBox(float x, float y, float w, float h, String label) {
    stroke(0, 255, 0);
    strokeWeight(5);
    fill(0, 50); // semi-transparent fill
    rect(x, y, w, h, 20); // rounded corners
    fill(0, 255, 0);
    textSize(40);
    textAlign(LEFT, CENTER);
    text(label, x + 20, y + h/2);
}

// Parse Arduino serial data
void parseData(String s) {
    // Vin: 12.45V Vout: 11.89V Iout: 0.34A Pot: 580 -> Increased
    String[] parts = splitTokens(s, " ");
    if (parts.length >= 10) {
        vin = parts[1].replace("V", "");
        vout = parts[3].replace("V", "");
        iout = parts[5].replace("A", "");
        potStatus = parts[9];
    }
}