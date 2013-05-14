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
        if (result.size() != this->rows * this->columns) {
            throw std::runtime_error("wrong size of the container to write the result to");
        }

        const size_t maxI = this->rows - 1;
        const size_t maxJ = this->columns - 1;
        size_t i = 0;
        size_t j = 0;

       /* while (true) {
            result.push_back((*this)[i, j]);

            if (j != maxJ) {
                ++j;
            }
            else {
                ++i;
            }

            if (i == maxI && j == maxJ) {
                break;
            }

            

            result.push_back((*this)[i, j]);
            ++j;
            result.push_back((*this)[i, j]);

            while (i != 0 && j != 0) {
                ++i;
                --j;
                result.push_back((*this)[i, j]);
            }

            if (i )
        }*/

        //TODO: implement

        unsigned int resultIndex = 0;
        for (size_t i = 0; i < this->rows; ++i) {
            for (size_t j = 0; j < this->columns; ++j) {
                result[resultIndex] =(*this)(i, j);
                ++resultIndex;
            }
        }
    }
}
}