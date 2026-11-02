#include "tensor.h"

const std::vector<float>& Tensor::GetData () const
{
    return Data_;    
};
const std::vector<size_t>& Tensor::GetShape() const
{
    return Shape_;
};
const std::vector<size_t>& Tensor::GetStrides() const
{
    return Strides_;
};

void Tensor::SetData(std::vector<float> data)
{
    size_t sizeOfTensor = 1;
    for(size_t i = 0; i < Shape_.size(); i++) 
    {
        sizeOfTensor = sizeOfTensor * Shape_[i];
    };
    if(data.size() == sizeOfTensor){ Data_ = data;}
    else{throw "incorrect shape or data of tensor"; };
};

void Tensor::Reshape(std::vector<size_t> shape)
{
    size_t sizeOfTensor = 1;
    ContiguousCheck();
    for(size_t i = 0; i < shape.size(); i++) 
    {
        sizeOfTensor = sizeOfTensor * shape[i];
    };
    if(sizeOfTensor == Data_.size() && IsContiguous_)
    {
        Shape_ = shape;
        calculateStrides();
    }
    else
    {throw "Invalid size of tensor or tensor is transposed";};
};

const float& Tensor::GetDataAt(std::vector<size_t> index) const
{
    if(index.size() != Shape_.size()){throw "Invalid index";};
    for(size_t i = 0; i < index.size(); i++)
    {
        if(index[i] >= Shape_[i]){throw "Invalid index";}
    };
    size_t findex{0};
    for(size_t i = 0; i < index.size(); i++)
    {
        findex += index.at(i)*Strides_.at(i);
    };
    return Data_.at(findex);
};

void Tensor::EditOnIndex(std::vector<size_t> index, float value)
{
    if(index.size() != Shape_.size()){throw "Invalid index";};
    for(size_t i = 0; i < index.size(); i++)
    {
        if(index[i] >= Shape_[i]){throw "Invalid index";}
    };
    size_t findex{0};
    for(size_t i = 0; i < index.size(); i++)
    {
        findex += index.at(i)*Strides_.at(i);
    };
    Data_.at(findex) = value;
};

void Tensor::Transpose(std::vector<size_t> permutations)
{
    if(permutations.size() != Shape_.size()){throw "Invalid argument";}
    std::vector<int> seen(permutations.size(), 0);
    for(size_t i = 0; i < permutations.size(); i++)
    {
        seen.at(permutations.at(i)) += 1;
    };
    for(size_t i = 0; i < permutations.size(); i++)
    {
        if(seen.at(i) > 1){throw "Invalid argument";}
    };
    std::vector<size_t> fShape(permutations.size(), 0);
    std::vector<size_t> fStrides(permutations.size(), 0);
    for(size_t i = 0; i < permutations.size(); i++)
    {
        fShape.at(i) = Shape_.at(permutations.at(i));
        fStrides.at(i) = Strides_.at(permutations.at(i));
    };
    Shape_ = fShape;
    Strides_ = fStrides;
    IsContiguous_ = false;
};

void Tensor::DangerSetData(std::vector<float> data)
{
    Data_ = data;
};

void Tensor::DangerReshape(std::vector<size_t> shape)
{
    Shape_ = shape;
    calculateStrides();
};

const float& Tensor::DangerGetDataAt(std::vector<size_t> index) const
{
    size_t findex = GetFlatIndex(index);
    return Data_.at(findex);
};

void Tensor::DangerEditOnIndex(std::vector<size_t> index, float value)
{
    size_t findex = GetFlatIndex(index);
    Data_.at(findex) = value;
};

void Tensor::DangerTranspose(std::vector<size_t> permutations)
{
    std::vector<size_t> fShape(permutations.size(), 0);
    std::vector<size_t> fStrides(permutations.size(), 0);
    for(size_t i = 0; i < permutations.size(); i++)
    {
        fShape.at(i) = Shape_.at(permutations.at(i));
        fStrides.at(i) = Strides_.at(permutations.at(i));
    };
    Shape_ = fShape;
    Strides_ = fStrides;
    IsContiguous_ = false;
};

const size_t& Tensor::GetFlatIndex(std::vector<size_t> index) const
{
    size_t findex{0};
    for(size_t i = 0; i < index.size(); i++)
    {
        findex += index.at(i)*Strides_.at(i);
    };
    return findex;
};

void Tensor::MakeContiguous()
{
    std::vector<size_t> indexVector;
    for (size_t i = 0; i < Data_.size(); ++i) {
        size_t n = i;
        size_t offset = 0;

        for (size_t j = Shape_.size(); j-- > 0;) {
            offset += (n % Shape_[j]) * Strides_[j];
            n /= Shape_[j];
        }

        indexVector.push_back(offset);
    }
    std::vector<float> resultData(Data_.size(), 0);
    for(size_t i = 0; i < indexVector.size(); i++)
    {
        resultData.at(i) = Data_.at(indexVector.at(i));
    };
    Data_ = std::move(resultData);
    calculateStrides();
    IsContiguous_ = true;
};

//операторы
Tensor operator + (const Tensor& t1, const Tensor& t2)
{
    if(t1.GetShape() != t2.GetShape()){throw "Invalid argument";};
    Tensor result{std::vector<float>(t1.GetData().size(), 1), t1.GetShape()};
    std::vector<size_t> index(t1.GetShape().size(), 0);
    for(size_t i = 0; i < t1.GetData().size(); i++)
    {
        for(size_t i1 = index.size() - 1; i1 > 0; i1 --)
        {
            if(index.at(i1) >= t1.GetShape().at(i1)){ index.at(i1) = 0; index.at(i1 - 1) += 1;};
        }; 
        result.EditOnIndex(index, t1.GetDataAt(index) + t2.GetDataAt(index));
        index.at(index.size() - 1) += 1;
    };
    return result;
};

Tensor operator - (const Tensor& t1, const Tensor& t2)
{
    if(t1.GetShape() != t2.GetShape()){throw "Invalid argument";};
    Tensor result{std::vector<float>(t1.GetData().size(), 1), t1.GetShape()};
    std::vector<size_t> index(t1.GetShape().size(), 0);
    for(size_t i = 0; i < t1.GetData().size(); i++)
    {
        for(size_t i1 = index.size() - 1; i1 > 0; i1 --)
        {
            if(index.at(i1) >= t1.GetShape().at(i1)){ index.at(i1) = 0; index.at(i1 - 1) += 1;};
        }; 
        result.EditOnIndex(index, t1.GetDataAt(index) - t2.GetDataAt(index));
        index.at(index.size() - 1) += 1;
    };
    return result;
};

Tensor Tensor::Hadamart (const Tensor& t1, const Tensor& t2) //умножение тензоров по Адамару
{
    if(t1.GetShape() != t2.GetShape()){throw "Invalid argument";};
    Tensor result{std::vector<float>(t1.GetData().size(), 1), t1.GetShape()};
    std::vector<size_t> index(t1.GetShape().size(), 0);
    for(size_t i = 0; i < t1.GetData().size(); i++)
    {
        for(size_t i1 = index.size() - 1; i1 > 0; i1 --)
        {
            if(index.at(i1) >= t1.GetShape().at(i1)){ index.at(i1) = 0; index.at(i1 - 1) += 1;};
        }; 
        result.EditOnIndex(index, t1.GetDataAt(index) * t2.GetDataAt(index));
        index.at(index.size() - 1) += 1;
    };
    return result;
};

Tensor Tensor::MatrixMult (const Tensor& t1, const Tensor& t2) // только для двумерных тензеров ака матриц
{
    if(t1.GetShape().size() != 2 || t2.GetShape().size() != 2){ throw "Invalid Argument(invalid shape of tensors)";};
    if(t1.GetShape().at(t1.GetShape().size() - 1) != t2.GetShape().at(0)){ throw "Invalid Argument(invalid shape of tensors)";};
    Tensor result{std::vector<float>(t1.GetShape().at(0)*t2.GetShape().at(t2.GetShape().size() - 1), 1), t1.GetShape()};
    // to be continued...
};