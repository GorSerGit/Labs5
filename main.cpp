/*
#include <iostream>
#include <vector>
#include "point.h"
#include "body.h"

using namespace std;

vector<Point> createDefaultSpace(int dimensions) {
    vector<Point> points;
    
    // Первая точка в начале координат
    vector<double> origin(dimensions, 0.0);
    points.emplace_back(origin);
    
    // Остальные точки на осях координат на расстоянии 10 единиц
    for (int i = 0; i < dimensions; ++i) {
        vector<double> coords(dimensions, 0.0);
        coords[i] = 10.0;
        points.emplace_back(coords);
    }
    
    return points;
}

int main() {
    int dimensions;
    cout << "Enter space dimensions: ";
    cin >> dimensions;

    // Создаем стандартное пространство
    vector<Point> points = createDefaultSpace(dimensions);
    
    cout << "Using default space with points at:\n";
    for (size_t i = 0; i < points.size(); ++i) {
        const auto& coords = points[i].getCoordinates();
        cout << "Point " << i+1 << ": (";
        for (size_t j = 0; j < coords.size(); ++j) {
            if (j > 0) cout << ", ";
            cout << coords[j];
        }
        cout << ")\n";
    }

    Body body(dimensions);
    
    while (true) {
        cout << "\nCurrent body coordinates: (";
        const auto& coords = body.getCoordinates();
        for (size_t i = 0; i < coords.size(); ++i) {
            if (i > 0) cout << ", ";
            cout << coords[i];
        }
        cout << ")\n";
        
        cout << "Enter axis to move (1-" << dimensions << ") or 0 to exit: ";
        int axis;
        cin >> axis;
        
        if (axis == 0) break;
        if (axis < 1 || axis > dimensions) {
            cout << "Invalid axis!\n";
            continue;
        }
        
        body.move(axis - 1);
        
        // Calculate and display error
        double error = body.calculateError(points);
        cout << "Calculation error: " << int(error) << endl;
    }
    
    return 0;
}*/

#include <iostream>
#include <vector>
#include "point.h"
#include "body.h"
#include "visualization.h"

using namespace std;

vector<Point> createDefaultSpace(int dimensions) {
    vector<Point> points;
    
    // Первая точка в начале координат
    vector<double> origin(dimensions, 0.0);
    points.emplace_back(origin);
    
    // Остальные точки на осях координат на расстоянии 10 единиц
    for (int i = 0; i < dimensions; ++i) {
        vector<double> coords(dimensions, 0.0);
        coords[i] = 10.0;
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
        // Оригинальный текстовый интерфейс для других размерностей
        while (true) {
            cout << "\nCurrent body coordinates: (";
            const auto& coords = body.getCoordinates();
            for (size_t i = 0; i < coords.size(); ++i) {
                if (i > 0) cout << ", ";
                cout << coords[i];
            }
            cout << ")\n";
            
            cout << "Enter axis to move (1-" << dimensions << ") or 0 to exit: ";
            int axis;
            cin >> axis;
            
            if (axis == 0) break;
            if (axis < 1 || axis > dimensions) {
                cout << "Invalid axis!\n";
                continue;
            }
            
            body.move(axis - 1);
            double error = body.calculateError(points);
            cout << "Calculation error: " << error << endl;
        }
    }
    
    return 0;
}