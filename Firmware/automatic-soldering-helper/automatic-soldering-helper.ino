// =====================================================
// AUTOMATIC SOLDERING HELPER v1
// =====================================================

#include <ESP32Servo.h>
#include <Preferences.h>

// ---------------- SERVOS ----------------

#define PIN_SHOULDER 25
#define PIN_ELBOW    26
#define PIN_WRIST    27
#define PIN_CLAMP    33

Servo shoulderServo;
Servo elbowServo;
Servo wristServo;
Servo clampServo;

// ---------------- POTS ----------------

#define POT_SHOULDER 34
#define POT_ELBOW    35
#define POT_WRIST    36

// ---------------- STEPPER ----------------

#define STEP_IN1 16
#define STEP_IN2 17
#define STEP_IN3 18
#define STEP_IN4 19

// ---------------- HOME ----------------

#define HOME_SWITCH 39

// External 10k pull-up:
// GPIO39 -> 10k -> 3.3V
// GPIO39 -> switch -> GND

// ---------------- BUTTONS ----------------

#define BTN_1 13
#define BTN_2 14
#define BTN_3 21
#define BTN_4 22
#define BTN_5 23

// ---------------- SERVO LIMITS ----------------

int SHOULDER_MIN = 30;
int SHOULDER_MAX = 150;

int ELBOW_MIN = 25;
int ELBOW_MAX = 155;

int WRIST_MIN = 20;
int WRIST_MAX = 160;

int CLAMP_OPEN = 45;
int CLAMP_CLOSED = 125;

// ---------------- HOME POSITION ----------------

int HOME_SHOULDER = 70;
int HOME_ELBOW = 120;
int HOME_WRIST = 90;

// ---------------- RETRACT POSITION ----------------

int RETRACT_SHOULDER = 55;
int RETRACT_ELBOW = 145;
int RETRACT_WRIST = 90;

// ---------------- CURRENT POSITION ----------------

int shoulderCurrent = HOME_SHOULDER;
int elbowCurrent = HOME_ELBOW;
int wristCurrent = HOME_WRIST;
int clampCurrent = CLAMP_OPEN;

// ---------------- TARGET POSITION ----------------

int shoulderTarget = HOME_SHOULDER;
int elbowTarget = HOME_ELBOW;
int wristTarget = HOME_WRIST;
int clampTarget = CLAMP_OPEN;

bool clampClosed = false;

// STEPPER

const byte stepSequence[8][4] = {
  {1,0,0,0},
  {1,1,0,0},
  {0,1,0,0},
  {0,1,1,0},
  {0,0,1,0},
  {0,0,1,1},
  {0,0,0,1},
  {1,0,0,1}
};

int stepIndex = 0;

long baseCurrent = 0;
long baseTarget = 0;

const long BASE_MIN = 0;
const long BASE_MAX = 4096;

unsigned long lastStepperMicros = 0;
const unsigned long STEPPER_SPEED = 1200;

// PRESETS

struct ArmPosition {
  long base;
  int shoulder;
  int elbow;
  int wrist;
  int clamp;
};

ArmPosition presets[3];

bool presetValid[3] = {
  false,
  false,
  false
};

Preferences prefs;

int saveSlot = 0;

// BUTTON SYSTEM

const int buttonPins[5] = {
  BTN_1,
  BTN_2,
  BTN_3,
  BTN_4,
  BTN_5
};

bool buttonState[5];
bool lastButtonState[5];

unsigned long pressStart[5];

const unsigned long HOLD_TIME = 1500;

// MODE

bool teachMode = false;

// TIMERS

unsigned long lastServoUpdate = 0;
unsigned long lastPotUpdate = 0;
unsigned long lastBaseJog = 0;

const int SERVO_INTERVAL = 15;
const int POT_INTERVAL = 30;

void setup() {

  Serial.begin(115200);

  // Servo setup

  shoulderServo.attach(PIN_SHOULDER, 500, 2400);
  elbowServo.attach(PIN_ELBOW, 500, 2400);
  wristServo.attach(PIN_WRIST, 500, 2400);
  clampServo.attach(PIN_CLAMP, 500, 2400);

  shoulderServo.write(shoulderCurrent);
  elbowServo.write(elbowCurrent);
  wristServo.write(wristCurrent);
  clampServo.write(clampCurrent);

  // Stepper pins

  pinMode(STEP_IN1, OUTPUT);
  pinMode(STEP_IN2, OUTPUT);
  pinMode(STEP_IN3, OUTPUT);
  pinMode(STEP_IN4, OUTPUT);

  // Buttons

  for (int i = 0; i < 5; i++) {

    pinMode(buttonPins[i], INPUT_PULLUP);

    buttonState[i] = HIGH;
    lastButtonState[i] = HIGH;
  }

  // Home sensor

  pinMode(HOME_SWITCH, INPUT);

  // Preferences storage

  prefs.begin("solderArm", false);

  loadPresets();

  delay(500);

  Serial.println();
  Serial.println("Automatic Soldering Helper");

  homeBase();

  Serial.println("Ready");
}


void loop() {

  readButtons();

  if (teachMode) {
    updateTeachMode();
  }

  updateServos();
  updateStepper();
}

// SMOOTH SERVO MOVEMENT

int moveOneDegree(int current, int target) {

  if (current < target) {
    return current + 1;
  }

  if (current > target) {
    return current - 1;
  }

  return current;
}

void updateServos() {

  if (millis() - lastServoUpdate < SERVO_INTERVAL) {
    return;
  }

  lastServoUpdate = millis();

  shoulderCurrent =
    moveOneDegree(shoulderCurrent, shoulderTarget);

  elbowCurrent =
    moveOneDegree(elbowCurrent, elbowTarget);

  wristCurrent =
    moveOneDegree(wristCurrent, wristTarget);

  clampCurrent =
    moveOneDegree(clampCurrent, clampTarget);

  shoulderServo.write(shoulderCurrent);
  elbowServo.write(elbowCurrent);
  wristServo.write(wristCurrent);
  clampServo.write(clampCurrent);
}

// STEPPER

void setStepperPhase(int index) {

  digitalWrite(STEP_IN1, stepSequence[index][0]);
  digitalWrite(STEP_IN2, stepSequence[index][1]);
  digitalWrite(STEP_IN3, stepSequence[index][2]);
  digitalWrite(STEP_IN4, stepSequence[index][3]);
}

void stepMotor(int direction) {

  stepIndex += direction;

  if (stepIndex > 7) {
    stepIndex = 0;
  }

  if (stepIndex < 0) {
    stepIndex = 7;
  }

  setStepperPhase(stepIndex);
}

void updateStepper() {

  if (baseCurrent == baseTarget) {
    return;
  }

  if (micros() - lastStepperMicros < STEPPER_SPEED) {
    return;
  }

  lastStepperMicros = micros();

  if (baseCurrent < baseTarget) {

    stepMotor(1);

    baseCurrent++;

  } else {

    stepMotor(-1);

    baseCurrent--;
  }
}

// HOMING

void homeBase() {

  Serial.println("Homing base...");

  long attempts = 0;

  while (
    digitalRead(HOME_SWITCH) == HIGH &&
    attempts < 5000
  ) {

    stepMotor(-1);

    delayMicroseconds(1800);

    attempts++;
  }

  if (digitalRead(HOME_SWITCH) == LOW) {

    baseCurrent = 0;
    baseTarget = 0;

    Serial.println("Home found");

  } else {

    Serial.println("Home switch not found");

    baseCurrent = 0;
    baseTarget = 0;
  }
}

// BUTTONS

void readButtons() {

  for (int i = 0; i < 5; i++) {

    bool state = digitalRead(buttonPins[i]);

    if (
      state == LOW &&
      lastButtonState[i] == HIGH
    ) {

      pressStart[i] = millis();
    }

    if (
      state == HIGH &&
      lastButtonState[i] == LOW
    ) {

      unsigned long held =
        millis() - pressStart[i];

      handleButton(i, held);
    }

    lastButtonState[i] = state;
  }
}

// BUTTON ACTIONS

void handleButton(int button, unsigned long held) {

  // BUTTON 1

  if (button == 0) {

    if (teachMode) {

      // Base left handled elsewhere

    } else {

      runPreset(0);
    }
  }

  // BUTTON 2

  if (button == 1) {

    if (teachMode) {

      // Base right handled elsewhere

    } else {

      runPreset(1);
    }
  }

  // BUTTON 3

  if (button == 2) {

    if (teachMode && held >= HOLD_TIME) {

      savePreset(saveSlot);

      saveSlot++;

      if (saveSlot > 2) {
        saveSlot = 0;
      }

    } else if (!teachMode) {

      runPreset(2);
    }
  }

  // BUTTON 4

  if (button == 3) {

    if (held >= HOLD_TIME) {

      teachMode = !teachMode;

      Serial.print("Teach mode: ");

      if (teachMode) {
        Serial.println("ON");
      } else {
        Serial.println("OFF");
      }

    } else {

      if (!teachMode) {
        goHome();
      }
    }
  }

  // BUTTON 5

  if (button == 4) {

    if (held >= HOLD_TIME) {

      emergencyRetract();

    } else {

      toggleClamp();
    }
  }
}

// TEACH MODE

void updateTeachMode() {

  // Pots

  if (millis() - lastPotUpdate >= POT_INTERVAL) {

    lastPotUpdate = millis();

    int s = analogRead(POT_SHOULDER);
    int e = analogRead(POT_ELBOW);
    int w = analogRead(POT_WRIST);

    shoulderTarget = map(
      s,
      0,
      4095,
      SHOULDER_MIN,
      SHOULDER_MAX
    );

    elbowTarget = map(
      e,
      0,
      4095,
      ELBOW_MIN,
      ELBOW_MAX
    );

    wristTarget = map(
      w,
      0,
      4095,
      WRIST_MIN,
      WRIST_MAX
    );
  }

  // Base jogging

  if (millis() - lastBaseJog > 25) {

    lastBaseJog = millis();

    // Button 1

    if (digitalRead(BTN_1) == LOW) {

      baseTarget -= 6;

      if (baseTarget < BASE_MIN) {
        baseTarget = BASE_MIN;
      }
    }

    // Button 2

    if (digitalRead(BTN_2) == LOW) {

      baseTarget += 6;

      if (baseTarget > BASE_MAX) {
        baseTarget = BASE_MAX;
      }
    }
  }
}

// CLAMP

void toggleClamp() {

  clampClosed = !clampClosed;

  if (clampClosed) {

    clampTarget = CLAMP_CLOSED;

    Serial.println("Clamp closed");

  } else {

    clampTarget = CLAMP_OPEN;

    Serial.println("Clamp open");
  }
}

// PRESETS

void savePreset(int slot) {

  presets[slot].base = baseCurrent;
  presets[slot].shoulder = shoulderCurrent;
  presets[slot].elbow = elbowCurrent;
  presets[slot].wrist = wristCurrent;
  presets[slot].clamp = clampCurrent;

  String key =
    "preset" + String(slot);

  prefs.putBytes(
    key.c_str(),
    &presets[slot],
    sizeof(ArmPosition)
  );

  String validKey =
    "valid" + String(slot);

  prefs.putBool(
    validKey.c_str(),
    true
  );

  presetValid[slot] = true;

  Serial.print("Saved preset ");
  Serial.println(slot + 1);
}

void loadPresets() {

  for (int i = 0; i < 3; i++) {

    String key =
      "preset" + String(i);

    String validKey =
      "valid" + String(i);

    presetValid[i] =
      prefs.getBool(
        validKey.c_str(),
        false
      );

    if (presetValid[i]) {

      prefs.getBytes(
        key.c_str(),
        &presets[i],
        sizeof(ArmPosition)
      );
    }
  }
}

void runPreset(int slot) {

  if (!presetValid[slot]) {

    Serial.print("Preset ");
    Serial.print(slot + 1);
    Serial.println(" not saved");

    return;
  }

  teachMode = false;

  baseTarget =
    constrain(
      presets[slot].base,
      BASE_MIN,
      BASE_MAX
    );

  shoulderTarget =
    constrain(
      presets[slot].shoulder,
      SHOULDER_MIN,
      SHOULDER_MAX
    );

  elbowTarget =
    constrain(
      presets[slot].elbow,
      ELBOW_MIN,
      ELBOW_MAX
    );

  wristTarget =
    constrain(
      presets[slot].wrist,
      WRIST_MIN,
      WRIST_MAX
    );

  clampTarget =
    presets[slot].clamp;

  Serial.print("Preset ");
  Serial.println(slot + 1);
}

// HOME

void goHome() {

  Serial.println("Going home");

  teachMode = false;

  clampTarget = CLAMP_OPEN;

  wristTarget = HOME_WRIST;

  elbowTarget = HOME_ELBOW;

  shoulderTarget = HOME_SHOULDER;

  baseTarget = 0;

  clampClosed = false;
}

// EMERGENCY RETRACT

void emergencyRetract() {

  Serial.println("EMERGENCY RETRACT");

  teachMode = false;

  clampTarget = CLAMP_OPEN;

  wristTarget = RETRACT_WRIST;

  elbowTarget = RETRACT_ELBOW;

  shoulderTarget = RETRACT_SHOULDER;

  baseTarget = 0;

  clampClosed = false;
}