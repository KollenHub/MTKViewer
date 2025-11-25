// I2DFromVtk.h
#pragma once

// DCMTK 头（根据你的安装路径可能需要调整）
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcuid.h>
#include <dcmtk/dcmdata/dcerror.h>
#include <dcmtk/dcmdata/libi2d/i2d.h>
#include <dcmtk/dcmdata/libi2d/i2dimgs.h> // I2DImgSource 基类（注意包含路径）
#include <vtkImageData.h>
#include <cstring>
#include <iostream>

// 说明：本类实现 I2DImgSource 的必要纯虚函数，直接从 vtkImageData 读取像素数据。
// 支持：VTK_UNSIGNED_CHAR（8 bit），1 通道（灰度）或 3 通道（RGB）。
// 如果你的 vtkImageData 是 16-bit 或 float，需要在这里做转换/量化。
class I2DFromVtk : public I2DImgSource
{
public:
    // 构造：传入指向 vtkImageData 的指针（不负责释放），可指定要读取的 z 切片 index（默认 0）
    I2DFromVtk(vtkImageData *img, int sliceIndex = 0)
        : m_img(img), m_slice(sliceIndex)
    {
    }

    virtual ~I2DFromVtk() override {}

    // 返回输入格式字符串（随意，但要实现）
    virtual OFString inputFormat() const override
    {
        return "VTK_IMAGE";
    }

    // 如果源是有损压缩图像（通常内存图像不是），可在这里返回 true 并设置方法名
    virtual OFCondition getLossyComprInfo(OFBool &srcEncodingLossy, OFString &srcLossyComprMethod) const override
    {
        srcEncodingLossy = OFFalse; // 内存 vtk 图像通常不是压缩的
        srcLossyComprMethod.clear();
        return EC_Normal;
    }

    /*
     * 关键函数：将像素数据和图像参数返回给 DCMTK
     *
     * 参数（按 DCMTK i2d 要求）：
     *   rows, cols, samplesPerPixel : 图像尺寸与通道数
     *   photoMetrInt : Photometric Interpretation ("MONOCHROME2" 或 "RGB")
     *   bitsAlloc,bitsStored,highBit : 位深信息
     *   pixelRepr : 是否有符号 (0=unsigned,1=signed)
     *   planConf : planar configuration (0 = interleaved)
     *   pixAspectH/pixAspectV : 像素纵横比（可选，常设为 1）
     *   pixData : 分配并返回的像素缓冲（必须用 new[] 分配，DCMTK 会在合适时机 delete[]）
     *   length : 返回缓冲长度（字节）
     *   ts : 建议的写入 transfer syntax（例如 EXS_LittleEndianExplicit）
     */
    virtual OFCondition readPixelData(
        Uint16 &rows,
        Uint16 &cols,
        Uint16 &samplesPerPixel,
        OFString &photoMetrInt,
        Uint16 &bitsAlloc,
        Uint16 &bitsStored,
        Uint16 &highBit,
        Uint16 &pixelRepr,
        Uint16 &planConf,
        Uint16 &pixAspectH,
        Uint16 &pixAspectV,
        char *&pixData,
        Uint32 &length,
        E_TransferSyntax &ts) override
    {
        if (!m_img)
            return EC_IllegalCall;

        // 获取尺寸
        int dims[3] = {0, 0, 0};
        m_img->GetDimensions(dims); // dims: [nx, ny, nz]
        int nx = dims[0], ny = dims[1], nz = dims[2];
        if (nx <= 0 || ny <= 0)
            return EC_IllegalCall;
        if (m_slice < 0 || m_slice >= nz)
            return EC_IllegalCall;

        // 通道数与数据类型
        int comps = m_img->GetNumberOfScalarComponents();
        int vtkType = m_img->GetScalarType(); // VTK_UNSIGNED_CHAR, VTK_UNSIGNED_SHORT, ...

        // TODO:这里读取8bit是因为读取的数据也是按照 8 bit读的，现在兼容高位效果不好暂时不处理
        //  我们当前只实现 8-bit unsigned（VTK_UNSIGNED_CHAR）
        if (vtkType != VTK_UNSIGNED_CHAR)
        {
            Logger::error("I2DFromVtk: 目前只支持 VTK_UNSIGNED_CHAR（8bit），请先转换数据或扩展实现");
            return EC_IllegalParameter;
        }

        // 填充基本属性
        rows = static_cast<Uint16>(ny);
        cols = static_cast<Uint16>(nx);
        samplesPerPixel = static_cast<Uint16>(comps);

        if (samplesPerPixel == 1)
            photoMetrInt = "MONOCHROME2";
        else if (samplesPerPixel == 3)
            photoMetrInt = "RGB";
        else
        {
            Logger::error("I2DFromVtk: 不支持的通道数:{}", comps);
            return EC_IllegalParameter;
        }

        // 位深：8-bit
        bitsAlloc = 8;
        bitsStored = 8;
        highBit = 7;
        pixelRepr = 0; // 无符号
        planConf = 0;  // 交织 RGBRGB...

        // 像素纵横比：使用 1/1（可根据 vtk spacing 调整）
        pixAspectH = 1;
        pixAspectV = 1;

        // 计算字节大小并分配（注意使用 new[]，DCMTK i2d 期望释放）
        size_t bufBytes = static_cast<size_t>(rows) * static_cast<size_t>(cols) * static_cast<size_t>(samplesPerPixel) * (bitsAlloc / 8);
        try
        {
            pixData = new char[bufBytes];
        }
        catch (const std::bad_alloc &)
        {
            pixData = nullptr;
            return EC_MemoryExhausted;
        }

        // 获取 vtk 指针到指定切片并逐行复制（更稳健）
        void *slicePtr = m_img->GetScalarPointer(0, 0, m_slice);
        if (!slicePtr)
        {
            delete[] pixData;
            pixData = nullptr;
            return EC_IllegalCall;
        }

        unsigned char *srcBase = static_cast<unsigned char *>(slicePtr);
        size_t rowBytes = static_cast<size_t>(cols) * static_cast<size_t>(samplesPerPixel) * (bitsAlloc / 8);
        for (int y = 0; y < ny; ++y)
        {
            size_t srcOffset = static_cast<size_t>(y) * static_cast<size_t>(nx) * static_cast<size_t>(samplesPerPixel) * (bitsAlloc / 8);
            std::memcpy(pixData + static_cast<size_t>(y) * rowBytes, srcBase + srcOffset, rowBytes);
        }

        length = static_cast<Uint32>(bufBytes);

        // 我们返回未压缩原始像素，推荐写出使用 Explicit Little Endian
        ts = EXS_LittleEndianExplicit;

        return EC_Normal;
    }

private:
    vtkImageData *m_img; // 不在类内管理生命周期，调用者负责保证有效
    int m_slice;
};
