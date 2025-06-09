#include <iostream>
#include <vector>
#include "point.h"
#include "body.h"
#include "visualization.h"
#include <random>

using namespace std;

vector<Point> createDefaultSpace(int dimensions) {
    int num_points = dimensions + 1;
    vector<Point> points;
    mt19937 gen(time(0));
    uniform_real_distribution<> dist(-10.0, 10.0);

    // Все точки со случайными координатами
    for (int i = 0; i < num_points; ++i) {
        vector<double> coords;
        for (int j = 0; j < dimensions; ++j) {
            coords.push_back(dist(gen));
        }
        points.emplace_back(coords);
    }
    
    return points;
}

int main() {
    int dimensions;
    cout << "Enter space dimensions: ";
    cin >> dimensions;

    vector<Point> points = createDefaultSpace(dimensions);
    Body body(dimensions);

    if (dimensions == 3) {
        Visualization visualizer;
        visualizer.run(points, body);
    } else {
        while (true) {
            cout << "Current body coordinates: (";
            const auto& coords = body.getCoordinates();
            for (size_t i = 0; i < coords.size(); ++i) {
                if (i > 0) cout << ", ";
                cout << coords[i];
            }
            cout << ")" << endl;

	    cout << "Initial points coordinates:"<<endl;
            for (const auto& point : points) {
                const auto& coords = point.getCoordinates();
                cout << "(";
                for (size_t i = 0; i < coords.size(); ++i) {
                    if (i > 0) cout << ", ";
                    cout << coords[i];
                }
                cout << ")" <<endl;;
            }
            
            cout << "Enter axis to move (1-" << dimensions << ") and direction (1 for positive, -1 for negative) separated by space, or 0 to exit: ";
            int axis, direction;
            cin >> axis;
            
            if (axis == 0) break;
            if (axis < 1 || axis > dimensions) {
                cout << "Invalid axis!\n";
                continue;
            }
            
            cin >> direction;
            body.move(axis - 1, direction);
            
            double error = body.calculateError(points);
            cout << "Calculation error: " << error << endl;
        }
    }
    
    return 0;
}