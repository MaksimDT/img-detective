#pragma once

#include "common/CommonInternal.h"
#include "utils/ContractUtils.h"
#include "utils/MemoryUtils.h"

#include <stdexcept>
#include <vector>

namespace ImgDetective {
namespace Core {

    template <typename T>
    class Matrix : NONCOPYABLE {
    public:
        Matrix(size_t rows, size_t columns);
        ~Matrix();

        T& operator () (size_t i, size_t j);
        const T& operator () (size_t i, size_t j) const;

        template <typename TResultCol>
        void ZigzagScan(REF TResultCol& result) const;

        size_t Rows() const {
            return this->rows;
        }

        size_t Columns() const {
            return this->columns;
        }
    private:
        T* GetItemAddr(size_t i, size_t j) const;

        T* data;
        size_t rows;
        size_t columns;
    };

    template <typename T>
    Matrix<T>::Matrix(size_t rows, size_t columns) {
        Utils::Contract::Assert(rows > 0);
        Utils::Contract::Assert(columns > 0);

        this->rows = rows;
        this->columns = columns;
        this->data = new T[rows * columns];
    }

    template <typename T>
    Matrix<T>::~Matrix() {
        Utils::Memory::SafeDeleteArray(this->data);
    }

    template <typename T>
    T& Matrix<T>::operator () (size_t i, size_t j) {
        T* dataPtr = GetItemAddr(i, j);
        return *dataPtr;
    }

    template <typename T>
    const T& Matrix<T>::operator () (size_t i, size_t j) const {
        T* dataPtr = GetItemAddr(i, j);
        return *dataPtr;
    }

    template <typename T>
    T* Matrix<T>::GetItemAddr(size_t i, size_t j) const {
        if (i < 0 || i >= this->rows) {
            throw std::runtime_error("row index is out of range");
        }

        if (j < 0 || j >= this->columns) {
            throw std::runtime_error("column index is out of range");
        }

        T* dataPtr = this->data + (i * this->columns + j);
        return dataPtr;
    }

    template <typename T>
    template <typename TResultCol>
    void Matrix<T>::ZigzagScan(REF TResultCol& result) const {
        Utils::Contract::Assert(this->rows == this->columns, "Zigzag scan algorithm is implemented only for square matrices");

        if (result.size() != this->rows * this->columns) {
            throw std::runtime_error("wrong size of the container to write the result to");
        }

        unsigned int N = this->rows - 1;
        unsigned int resultItemIndex = 0;

        //Sum of element's indexes (i + j) on the given matrix diagonal is a const value
        //c variable stands for (i + j).
        for (int c = 0; c <= 2 * N; ++c) {
            int i;
            int j;
            int di;
            int dj;

            if (c % 2 == 0) {
                //i + j is even
                if (c > N) {
                    i = N;
                    j = c - N;
                }
                else {
                    i = c;
                    j = 0;
                }
                //go to the right and up
                di = -1;
                dj = 1;
            }
            else {
                //i + j is odd
                if (c > N) {
                    i = c - N;
                    j = N;
                }
                else {
                    i = 0;
                    j = c;
                }
                //go to the left and down
                di = 1;
                dj = -1;
            }

            while (0 <= i && i <= N && 0 <= j && j <= N) {
                result[resultItemIndex] = (*this)(i, j);
                ++resultItemIndex;
                i += di;
                j += dj;
            }
        }
    }
}
}