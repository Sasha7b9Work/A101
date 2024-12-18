// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "Editor/Painter/Canvas.h"
#include "Editor/Painter/Zoomer.h"
#include "Utils/MyMath.h"

#include <vector>

#pragma warning(push, 0)
#include <wx/textfile.h>
#pragma warning(pop)

/*
    ����� ������������� ����� �������
*/


struct Align
{
    enum E
    {
        Left,
        Right,
        LeftTop,
        LeftDown,
        RightTop,
        RightDown
    };
};


struct Marker
{
    static const int SIZE = 5;
};


struct Point
{
    static const int SIZE = 3;

    static const uint16 MIN = (uint16)0;
    static const uint16 MAX = (uint16)((1 << 12) - 1);
    static const uint16 AVE = (uint16)(MAX / 2);

    static const int AMOUNT = 8192;

    Point() : Point((uint16)0, (uint16)0) { };

    static Point FromData(uint16 p, uint16 d)
    {
        return Point(p, d);
    }

    static Point FromCanvas(int canvasX, int canvasY)
    {
        return Point(canvasX, canvasY);
    }

private:

    Point(int canvasX, int canvasY)
    {
        pos = Math::Round<uint16>((float)((float)canvasX + (float)Zoomer::IndexFirsPoint() * PixelsInPointX()) / PixelsInPointX());
        SetCanvasY(canvasY);
    }

    Point(uint16 p, uint16 d) : pos(p), data(d) {};

public:

    void SetData(uint16 p, uint16 d)
    {
        *this = Point(p, d);
    };

    void SetCanvasY(int canvasY)
    {
        data = (uint16)(Point::MAX - Math::Round<uint16>((float)(canvasY - Grid::Y()) / PixelsInDiscretY()));
    }

    uint16 pos;
    uint16 data;
    bool operator < (const Point &point) const
    {
        return (pos < point.pos);
    }
    // ���������� true, ���� ������ ���� ��������� ��� ���� ������
    double DistanceFromCanvas(int canvasX, int canvasY)
    {
        int dX = canvasX - CanvasX();

        int dY = canvasY - CanvasY();

        return std::sqrt(dX * dX + dY * dY);
    }
    // ������� �� ����������� - �������� �� �����
    static float PixelsInPointX()
    {
        return (float)Grid::Width() / (float)Zoomer::NumberDrawingPoints();
    }
    // ������� �� ��������� - �������� �� �������
    static float PixelsInDiscretY()
    {
        return (float)Grid::Height() / (float)MAX;
    }

    // ����������� ���������� ���� � ����� [0 ... (Point::AMOUNT - 1)]
    static int CanvasToPointX(int mouseX)
    {
        return Math::Round<int>((float)mouseX / PixelsInPointX());
    }

    static int PointToCanvasX(int x)
    {
        return Math::Round<int>((float)x * PixelsInPointX());
    }

    // ���������� ��������� �����
    void DrawParameters();

    // ���������� ���������� ��� ������ ����������
    wxPoint CalculateCoordParameters();

    // ���������� X ����� �� ������
    int CanvasX() const;

    // ���������� Y ����� �� ������
    int CanvasY() const;
};


class Form
{
public:
    Form();
    ~Form();
    // �������� ������
    void Clear();

    // �������� ������ � ������������ ����
    void SetMarkerInMouseCoord(int mouseX, int mouseY);

    // �������� ����� � ����������� ����������
    void SetPointInRealCoord(uint16 pos, uint16 data);

    // ������� ����� ���� �������
    void SetPoint(Point point);

    // �������� ����� � ������� pos
    void SetPointInPosition(uint16 pos);

    // ���������� true, ���� ������ ���� ��������� ��� ������������ ������. Pressed - ������ �� ������ ����
    bool ExistMarker(int mouseX, int mouseY, bool pressed, uint16 *index = nullptr, uint16 *value = nullptr);

    // ������� ����� � ������� ����
    void RemoveCurrentMarker();

    // ����������� ������ � ����� �������
    void MoveMarker(int canvasX, int canvasY);

    // ��������� �����
    void AlignPoint(Align::E align);

    void Draw();

    void UndoHistory();

    void RedoHistory();

    // ���������� �������������� �����, ������� ����� ���������� ������ ��������. 
    void SetAdditionForm(const uint16 data[Point::AMOUNT]);

    // ���������� �������� �����
    void SetMainForm(const uint16 data[Point::AMOUNT], const std::vector<Point> *points);

    bool IsEquals(const Form *form) const;

    void SaveToFile(wxTextFile &file);

    void LoadFromFile(wxTextFile &file);

private:

    // ������, ������� ��� �������� � ������
    uint16 data[Point::AMOUNT];

    // �����, ������������ �����. �������� � ������� ��������� ������� (pos = [0 ... Point::AMOUNT], data = [Point::MIN ... Point::MAX])
    std::vector<Point> markers;

    // ����� �������� ������ �����, ������� ��������� � ������� ������
    uint iCurMarker = 0;

    // ���������� index ����� � ������� pos. 0xFFFFFFFF, ���� ����� � ���� ������� ���
    uint PointInPosition(uint16 pos);

    // ���������� �������� � point �����
    void CalculateNeighboringPoints(const Point &point);

    // ������� ��������������� �����, ������������� ����� pos1 � pos2
    void LinearInterpolation(uint16 pos1, uint16 pos2);

    // ��������������� ����� ����� �� ����� � �������� index �� points
    void LinearInterpolationLeft(uint index);

    // ��������������� ����� ������ �� ����� � �������� index �� points
    void LinearInterpolationRight(uint index);
};


extern Form *TheForm;
