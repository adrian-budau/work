#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>

using namespace std;

double eps = 1e-9;

int cmp(double a, double b) {
    if ( a + eps < b)
        return -1;
    if (b + eps < a)
        return 1;
    return 0;
}

double a = 0.0008876759111816247;
double b = -0.13593150250927571;
double c = 6.23;

double throttleSpeed(double speed, double throttle) {
    return (a * speed * speed + b * speed + c) * 0.5 * throttle / 100;
}

double maxDecrease = 5.8;

bool canPrint = true;

void normalizeBrake(int &x) {
    if (x < 0)
        x = 0;
    if (x > 100)
        x = 100;
}

double maxSpeedReduction(double distance, double nowSpeed) {
    if (canPrint)
        cerr << "For distance " << distance << " meters and speed " << nowSpeed << "km/h we have ";
    double speed = nowSpeed * 1000 / 3600;

    double delta = speed * speed - 2 * maxDecrease * distance;
    if (delta < 0) {
        if (canPrint)
            cerr << "-1\n";
        canPrint = false;
        return -1;
    }

    delta = sqrt(delta);
    double t = (-speed + delta) / maxDecrease;
    if (canPrint)
        cerr << " time(" << t << ") ";

    if (canPrint)
        cerr << (speed - t * maxDecrease) * 3600 / 1000 << "km/h \n";
    canPrint = false;
    return (speed - t * maxDecrease) * 3600 / 1000;
}

int maxBrake(double distance, double speed, double needSpeed) {
    double realSpeed = speed * 1000 / 3600;
    double realNeedSpeed = needSpeed * 1000 / 3600;

    double t = 2 * distance / (3 * realSpeed - realNeedSpeed);
    double a = (distance - t * realSpeed) / t / t * 2;
    return (int)(a / maxDecrease * 100 + 0.5);
}

int main() {
    double speed = 0, distance = 0, time = 0;
    double speedLimit = 0, distanceToNext = 0, nextLimit = 0;
    double lightDistance = 0; string lightState; double remainingTime;
    double nextCarDistance = 0, nextCarSpeed = 0;
    int iterations = 0;
    do {
        double lastLightDistance = lightDistance;
        string S; cin >> S;
        while (S != "update") {
            if (S == "speed")
                cin >> speed;
            else if (S == "distance")
                cin>> distance;
            else if (S == "time")
                cin >> time;
            else if (S == "speedlimit")
                cin >> speedLimit >> distanceToNext >> nextLimit;
            else if (S == "trafficlight") {
                cin >> lightDistance;
                string input;
                getline(cin, input);
                istringstream reader(input);
                if (!(reader >> lightState)) {
                    lightState = "Green";
                    lightDistance = 100000;
                    remainingTime = 100000;
                } else {
                    lightDistance -= 0.5;
                    reader >> remainingTime;
                }
           } else if (S == "nextcar") {
               cin >> nextCarDistance >> nextCarSpeed;
               if (cmp(nextCarDistance, 0) == 0) {
                   nextCarDistance = 10000000;
                   nextCarSpeed = 100000;
               }
           }

            cin >> S;
        }

        if (cmp(nextLimit, 0) == 0)
            nextLimit = 10000;

        if (distance >= 1500.0) {
            if (cmp(speed, 0) == 0) {
                break;
            }

            cout << "throttle 0\n";
            cout << "brake 100\n";
            cout.flush();
            continue;
        }

        int throttle = 0.0, brake = 0.0;
        if (nextLimit > speedLimit || maxSpeedReduction(max(distanceToNext - 30, 0.0), speed) < nextLimit) {
            throttle = 40;
            while (throttleSpeed(speed, throttle) + speed > speedLimit)
                --throttle;
        } else {
            if (speed < nextLimit) {
                throttle = 40;
                while (throttleSpeed(speed, throttle) + speed > nextLimit)
                    --throttle;
            } else { // Let's not brake too much
                if (speed < nextLimit + 3) {
                    throttle = 0;
                    brake = 0;
                } else {
                    throttle = 0;
                    brake = 100;
                }
            }
        }

        ++iterations;
        if (lastLightDistance < lightDistance || iterations % 5 == 0)
            canPrint = true;

        // lights
        double nextSpeed = max(throttleSpeed(speed, throttle) + speed - brake / 100.0 * 2.1, 0.0);

        double timeToLight = (lightDistance - 2) / (nextSpeed * 1000 / 3600 + 0.0001);

        int max_Brake = brake;
        if (lightState == "Green") {
            if (cmp(timeToLight, remainingTime) < 0)
                ;
            else {
                throttle = 0;
                brake = maxBrake(lightDistance, speed, 0.0) * 0.5;
                normalizeBrake(brake);
            }
        } else {
            if (iterations % 5 == 0)
                cerr << timeToLight << " vs " << remainingTime << "\n";
            if (cmp(timeToLight, remainingTime) > 0)
                ;
            else if (cmp(lightDistance / (speed * 1000 / 3600 + 0.0000001), remainingTime) >= 0) {
                throttle = 0;
                brake = 0;
            } else {
                throttle = 0;
                brake = maxBrake(lightDistance, speed, 0) * 0.5;
                normalizeBrake(brake);
            }
        }

        brake = max(brake, max_Brake);
        if (nextCarDistance < 20 && nextCarSpeed < nextSpeed && brake == 0) {
            throttle = 0;
            brake = (nextSpeed - nextCarSpeed) * 3;
            normalizeBrake(brake);
        }


        if (iterations < 30) {
            throttle = 0;
            brake = 0;
        }

        cout << "throttle " << throttle << "\n";
        cout << "brake " << brake << "\n";
        cout.flush();
    } while (true);
    return 0;
}
