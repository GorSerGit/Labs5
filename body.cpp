#include "body.h"
#include <vector>
#include <stdexcept>
#include <cmath>
#include <iostream>

Body::Body(int dimensions) {
    coordinates.resize(dimensions, 0.0);
}

void Body::move(int axis, float direction = 1.0f) {
    if (axis >= 0 && axis < static_cast<int>(coordinates.size())) {
        coordinates[axis] += direction * 1.0f; // direction может быть 1 или -1
    }
}

const std::vector<double>& Body::getCoordinates() const {
    return coordinates;
}

std::vector<double> Body::calculateDistances(const std::vector<Point>& points) const {
    std::vector<double> distances;
    Point bodyPoint(coordinates);
    
    for (const auto& point : points) {
        distances.push_back(bodyPoint.distanceTo(point));
    }
    
    return distances;
}

std::vector<double> Body::triangulate(const std::vector<Point>& points, const std::vector<double>& distances) const {
    size_t n = coordinates.size();
    
    if (points.size() != n + 1) {
        throw std::invalid_argument("Number of points must be n+1 for n-dimensional space");
    }

    std::vector<std::vector<double>> A(n, std::vector<double>(n));
    std::vector<double> b(n);

    // Используем первую точку как опорную
    const auto& p0 = points[0].getCoordinates();
    double d0 = distances[0];

    for (size_t i = 0; i < n; ++i) {
        const auto& pi = points[i + 1].getCoordinates();
        double di = distances[i + 1];

        // Заполнение матрицы A
        for (size_t j = 0; j < n; ++j) {
            A[i][j] = 2 * (pi[j] - p0[j]);
        }

        // Заполнение вектора b
        double sum_p0 = 0.0, sum_pi = 0.0;
        for (size_t j = 0; j < n; ++j) {
            sum_p0 += p0[j] * p0[j];
            sum_pi += pi[j] * pi[j];
        }
        b[i] = sum_pi - sum_p0 + (d0 * d0) - (di * di);
    }

    // Применение метода Гаусса для решения системы линейных уравнений
    for (size_t i = 0; i < n; ++i) {
        // Поиск ведущего элемента
        size_t pivot = i;
        for (size_t j = i + 1; j < n; ++j) {
            if (std::abs(A[j][i]) > std::abs(A[pivot][i])) {
                pivot = j;
            }
        }

        if (pivot != i) {
            std::swap(A[i], A[pivot]);
            std::swap(b[i], b[pivot]);
        }

        // Элиминирование
        for (size_t j = i + 1; j < n; ++j) {
            double factor = A[j][i] / A[i][i];
            for (size_t k = i; k < n; ++k) {
                A[j][k] -= factor * A[i][k];
            }
            b[j] -= factor * b[i];
        }
    }

    // Обратная подстановка
    std::vector<double> result(n);
    for (int i = n - 1; i >= 0; --i) {
        result[i] = b[i];
        for (size_t j = i + 1; j < n; ++j) {
            result[i] -= A[i][j] * result[j];
        }
        result[i] /= A[i][i];
    }

    return result;
}

double Body::calculateError(const std::vector<Point>& points) const {
    auto distances = calculateDistances(points);
    auto calculatedCoords = triangulate(points, distances);
    std::cout<<"Prediction coordinates: ";
    double error = 0.0;
    for (size_t i = 0; i < coordinates.size(); ++i) {
        double diff = coordinates[i] - calculatedCoords[i];
	std::cout<<calculatedCoords[i]<<" ";
        error += diff * diff;
    }
    std::cout<<std::endl;
    
    return error;
}