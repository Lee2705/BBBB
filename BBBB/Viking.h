#pragma once
#ifndef HNEFATAFL_H
#define HNEFATAFL_H
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <iomanip>
#include <algorithm>
#include <random>
#include <chrono>
using namespace std;
enum Phe { TRONG = 0, XAM_LUOC = 1, PHONG_THU = 2 };
enum LoaiQuan { KHONG = 0, LINH_XAM_LUOC = 1, CAN_VE = 2, VUA = 3 };
class OCo {
public:
    LoaiQuan quan;
    Phe phe;
    bool laGoc;
    bool laNgaiVang;
    OCo();
};
class BoPhanPhanTich {
public:
    static int TinhKhoangCach(int x1, int y1, int x2, int y2);
    static float DanhGiaTheTran(OCo luoi[9][9], int slXamLuoc, int slPhongThu);
    static void HienThiTiLeThang(float diem);
};
struct NuocDi {
    int x1, y1, x2, y2;
};
class BotViking {
public:
    // Them bien ngau nhien de tao su da dang giua cac van
    mt19937 rng;
    int vanSo; // dem so van da choi de tang nhieu phe nhieu
    BotViking();
    NuocDi TimNuocDiTotNhat(class BanCoViking& game, Phe pheCuaBot);
    float DanhGiaNuocDiBot(BanCoViking& game, Phe pheCuaBot, const NuocDi& nd);
    NuocDi TimNuocDiTotNhatSau(BanCoViking& game, Phe phe);
    int DemQuanKeBen(BanCoViking& game, int x, int y, Phe phe);
    bool KiemTraCuaRa(BanCoViking& game, Phe pheCuaBot);
};
class BanCoViking {
    friend class BotViking;
private:
    int slXamLuoc;
    int slPhongThu;
    Phe luotDi;
    bool troChoiKetThuc;
    void KiemTraKepThit(int x, int y);
    bool LaKeThu(int x, int y, Phe pheHienTai);
    bool LaDongMinhHoacGoc(int x, int y, Phe pheHienTai);
public:
    OCo luoi[9][9];
    bool DuongDiHopLe(int x1, int y1, int x2, int y2);
    BanCoViking();
    void KhoiTao(int doKho);
    void VeBanCo();
    void ThucHienNuocDi(int x1, int y1, int x2, int y2);
    bool DaKetThuc();
    Phe layLuotDi() { return luotDi; }
};
class ChuTro {
public:
    int ThietLapLuatChoi();
};
#endif