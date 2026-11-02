#include <vector>
#pragma once

class Tensor{
    private:
        std::vector<float> Data_;
        std::vector<size_t> Shape_;
        std::vector<size_t> Strides_;
        bool IsContiguous_ = true;
        void ContiguousCheck() //пока нигде не вызывается
        {
            if(Strides_ == calculateStridesReturn()) //TODO: сделать нормальную проверку а не просто через strides
            {
                IsContiguous_ = true;
            }
            else
            {
                IsContiguous_ = false;
            };
        };
        void calculateStrides() // вызывается в сеттере формы для изменения Strides_
        {
            size_t size = Shape_.size();
            std::vector<size_t> resultStrides(size, 1);
        
            for(size_t i = 1; i < size; i++)
            {
                resultStrides.at(size - i - 1) = resultStrides.at(size - i) * Shape_.at(size - i);
            };

            Strides_ = resultStrides;
        };
        const std::vector<size_t>& calculateStridesReturn()
        {
            size_t size = Shape_.size();
            std::vector<size_t> resultStrides(size, 1);
        
            for(size_t i = 1; i < size; i++)
            {
                resultStrides.at(size - i - 1) = resultStrides.at(size - i) * Shape_.at(size - i);
            };

            return resultStrides;
        };
    public:
        Tensor(std::vector<float> data, std::vector<size_t> shape)
        {
            size_t sizeOfTensor = 1;
            for(size_t i = 0; i < shape.size(); i++) 
            {
                sizeOfTensor = sizeOfTensor * shape[i];
            };
            if(sizeOfTensor == data.size())
            {
                Data_ = data;
                Shape_ = shape;
                calculateStrides();
                IsContiguous_ = true;
            }
            else
            {
                throw "Invalid arguments";
            };
        };

        // геттеры
        const std::vector<float>& GetData () const;
        const std::vector<size_t>& GetShape() const;
        const std::vector<size_t>& GetStrides() const;
        const float& GetDataAt(std::vector<size_t> index) const;

        //сеттеры
        void SetData(std::vector<float> data); // только если количество эл совпадает с shape
        void Reshape(std::vector<size_t> shape); // тоже проверить валидность преобразования или сделать автообрезание/дополнение даты
        void EditOnIndex(std::vector<size_t> index, float value);
        void Transpose(std::vector<size_t> permutations);
        void MakeContiguous();
        
        //опасные методы без проверок
        void DangerSetData(std::vector<float> data);
        void DangerReshape(std::vector<size_t> shape); 
        void DangerEditOnIndex(std::vector<size_t> index, float value);
        void DangerTranspose(std::vector<size_t> permutations);
        const float& DangerGetDataAt(std::vector<size_t> index) const;

        //инструменты
        const size_t& GetFlatIndex(std::vector<size_t> index) const;
};

