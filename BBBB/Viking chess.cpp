#include "Viking.h"

// ================= BASIC =================
OCo::OCo() {
    quan = KHONG;
    phe = TRONG;
    laGoc = false;
    laNgaiVang = false;
}

int BoPhanPhanTich::TinhKhoangCach(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

float BoPhanPhanTich::DanhGiaTheTran(OCo luoi[9][9], int slXamLuoc, int slPhongThu) {
    float diem = 0;
    int vx = 4, vy = 4;

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (luoi[i][j].quan == VUA) {
                vx = i; vy = j;
            }
        }
    }

    int d1 = TinhKhoangCach(vx, vy, 0, 0);
    int d2 = TinhKhoangCach(vx, vy, 0, 8);
    int d3 = TinhKhoangCach(vx, vy, 8, 0);
    int d4 = TinhKhoangCach(vx, vy, 8, 8);
    int kcGanNhat = min(min(d1, d2), min(d3, d4));

    diem += (10 - kcGanNhat) * 4.0;
    diem += slPhongThu * 5.0;
    diem -= slXamLuoc * 3.5;

    return diem;
}

void BoPhanPhanTich::HienThiTiLeThang(float diem) {
    float phanTramVuaThang = 100 / (1 + exp(-diem * 0.1));
    cout << ">> Ti le thang Phe Vua [" << fixed << setprecision(1) << phanTramVuaThang
        << "%] - Phe Xam Luoc [" << 100 - phanTramVuaThang << "%] <<";
}

// ================= BAN CO =================
BanCoViking::BanCoViking() {
    slXamLuoc = 16;
    slPhongThu = 9;
    luotDi = XAM_LUOC;
    troChoiKetThuc = false;
}

void BanCoViking::KhoiTao(int doKho) {
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            luoi[i][j] = OCo();

    luoi[0][0].laGoc = luoi[0][8].laGoc = luoi[8][0].laGoc = luoi[8][8].laGoc = true;
    luoi[4][4].laNgaiVang = true;

    luoi[4][4].quan = VUA; luoi[4][4].phe = PHONG_THU;

    int cv[8][2] = { {3,4},{5,4},{4,3},{4,5},{2,4},{6,4},{4,2},{4,6} };
    for (auto p : cv) { luoi[p[0]][p[1]].quan = CAN_VE; luoi[p[0]][p[1]].phe = PHONG_THU; }

    int xl_de[12][2] = { {0,3},{0,4},{0,5},{8,3},{8,4},{8,5},{3,0},{4,0},{5,0},{3,8},{4,8},{5,8} };
    int xl_kho[16][2] = { {0,3},{0,4},{0,5},{1,4},{8,3},{8,4},{8,5},{7,4},{3,0},{4,0},{5,0},{4,1},{3,8},{4,8},{5,8},{4,7} };

    if (doKho == 1) {
        slXamLuoc = 12;
        for (auto p : xl_de) { luoi[p[0]][p[1]].quan = LINH_XAM_LUOC; luoi[p[0]][p[1]].phe = XAM_LUOC; }
    }
    else {
        slXamLuoc = 16;
        for (auto p : xl_kho) { luoi[p[0]][p[1]].quan = LINH_XAM_LUOC; luoi[p[0]][p[1]].phe = XAM_LUOC; }
    }
}

bool BanCoViking::DuongDiHopLe(int x1, int y1, int x2, int y2) {
    if (x1 != x2 && y1 != y2) return false;
    if (luoi[x2][y2].phe != TRONG) return false;

    if (luoi[x2][y2].laGoc && luoi[x1][y1].quan != VUA) return false;
    if (luoi[x2][y2].laNgaiVang && luoi[x1][y1].quan != VUA) return false;

    if (x1 == x2) {
        int minY = min(y1, y2), maxY = max(y1, y2);
        for (int i = minY + 1; i < maxY; i++)
            if (luoi[x1][i].phe != TRONG) return false;
    }
    else {
        int minX = min(x1, x2), maxX = max(x1, x2);
        for (int i = minX + 1; i < maxX; i++)
            if (luoi[i][y1].phe != TRONG) return false;
    }
    return true;
}

bool BanCoViking::LaKeThu(int x, int y, Phe pheHienTai) {
    return (luoi[x][y].phe != TRONG && luoi[x][y].phe != pheHienTai);
}

bool BanCoViking::LaDongMinhHoacGoc(int x, int y, Phe pheHienTai) {
    if (luoi[x][y].laGoc || luoi[x][y].laNgaiVang) return true;
    return luoi[x][y].phe == pheHienTai;
}

void BanCoViking::KiemTraKepThit(int x, int y) {
    Phe pheCuaToi = luoi[x][y].phe;

    if (x >= 2 && LaKeThu(x - 1, y, pheCuaToi) && LaDongMinhHoacGoc(x - 2, y, pheCuaToi)) {
        if (luoi[x - 1][y].quan == VUA) troChoiKetThuc = true;
        luoi[x - 1][y] = OCo();
        (pheCuaToi == XAM_LUOC) ? slPhongThu-- : slXamLuoc--;
        cout << "\n>>> BUUUM! Da khep vong vay, an 1 quan tai (" << x - 1 << "," << y << ")!\n";
    }

    if (x <= 6 && LaKeThu(x + 1, y, pheCuaToi) && LaDongMinhHoacGoc(x + 2, y, pheCuaToi)) {
        if (luoi[x + 1][y].quan == VUA) troChoiKetThuc = true;
        luoi[x + 1][y] = OCo();
        (pheCuaToi == XAM_LUOC) ? slPhongThu-- : slXamLuoc--;
        cout << "\n>>> BUUUM! Da khep vong vay, an 1 quan tai (" << x + 1 << "," << y << ")!\n";
    }

    if (y >= 2 && LaKeThu(x, y - 1, pheCuaToi) && LaDongMinhHoacGoc(x, y - 2, pheCuaToi)) {
        if (luoi[x][y - 1].quan == VUA) troChoiKetThuc = true;
        luoi[x][y - 1] = OCo();
        (pheCuaToi == XAM_LUOC) ? slPhongThu-- : slXamLuoc--;
        cout << "\n>>> BUUUM! Da khep vong vay, an 1 quan tai (" << x << "," << y - 1 << ")!\n";
    }

    if (y <= 6 && LaKeThu(x, y + 1, pheCuaToi) && LaDongMinhHoacGoc(x, y + 2, pheCuaToi)) {
        if (luoi[x][y + 1].quan == VUA) troChoiKetThuc = true;
        luoi[x][y + 1] = OCo();
        (pheCuaToi == XAM_LUOC) ? slPhongThu-- : slXamLuoc--;
        cout << "\n>>> BUUUM! Da khep vong vay, an 1 quan tai (" << x << "," << y + 1 << ")!\n";
    }
}

void BanCoViking::VeBanCo() {
    cout << "\n   0  1  2  3  4  5  6  7  8";
    for (int i = 0; i < 9; i++) {
        cout << "\n" << i << " ";
        for (int j = 0; j < 9; j++) {
            if (luoi[i][j].quan == VUA) cout << " V ";
            else if (luoi[i][j].quan == CAN_VE) cout << " o ";
            else if (luoi[i][j].quan == LINH_XAM_LUOC) cout << " x ";
            else if (luoi[i][j].laGoc || luoi[i][j].laNgaiVang) cout << " + ";
            else cout << " . ";
        }
        if (i == 1) {
            cout << "   [Thong ke] Xam luoc (x) con: " << slXamLuoc;
        }
        else if (i == 2) {
            cout << "   [Thong ke] Phong thu (V,o) con: " << slPhongThu;
        }
        else if (i == 4) {
            cout << "   ";
            float diem = BoPhanPhanTich::DanhGiaTheTran(luoi, slXamLuoc, slPhongThu);
            BoPhanPhanTich::HienThiTiLeThang(diem);
        }
    }
    cout << "\n--------------------------------------------------------------\n";
}

void BanCoViking::ThucHienNuocDi(int x1, int y1, int x2, int y2) {
    if (luoi[x1][y1].phe != luotDi) {
        cout << "Khong phai luot cua ban hoac chon sai quan!\n";
        return;
    }

    if (!DuongDiHopLe(x1, y1, x2, y2)) {
        cout << "Nuoc di khong hop le (Bi chan hoac di cheo)!\n";
        return;
    }

    bool destLaGoc = luoi[x2][y2].laGoc;
    bool destLaNgaiVang = luoi[x2][y2].laNgaiVang;

    luoi[x2][y2] = luoi[x1][y1];
    luoi[x2][y2].laGoc = destLaGoc;
    luoi[x2][y2].laNgaiVang = destLaNgaiVang;

    luoi[x1][y1] = OCo();

    if (luoi[x2][y2].quan == VUA && luoi[x2][y2].laGoc) {
        cout << "\nVUA DA DEN GOC! PHE PHONG THU CHIEN THANG!\n";
        troChoiKetThuc = true;
        return;
    }

    KiemTraKepThit(x2, y2);

    if (troChoiKetThuc && luotDi == XAM_LUOC) {
        cout << "\nVUA DA BI BAT! PHE XAM LUOC CHIEN THANG!\n";
    }

    luotDi = (luotDi == XAM_LUOC) ? PHONG_THU : XAM_LUOC;
}

bool BanCoViking::DaKetThuc() {
    return troChoiKetThuc;
}

int ChuTro::ThietLapLuatChoi() {
    cout << "=== MENU CHU TRO (ADMIN) ===\n";
    cout << "1. Che do Dan thuong (12 Xam luoc - De cho Vua)\n";
    cout << "2. Che do Chien binh (16 Xam luoc - Can bang)\n";
    cout << "Chon do kho (1/2): ";
    int luaChon;
    cin >> luaChon;
    return luaChon;
}

// ================= BOT (NANG CAP) =================

BotViking::BotViking() {
    // Khoi tao seed ngau nhien theo thoi gian thuc -> moi van khac nhau
    rng.seed((unsigned)chrono::steady_clock::now().time_since_epoch().count());
    vanSo = 0;
}

void SaoChepBanCo(OCo nguon[9][9], OCo dich[9][9]) {
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            dich[i][j] = nguon[i][j];
}

int BotViking::DemQuanKeBen(BanCoViking& game, int x, int y, Phe phe) {
    int dx[] = { -1,1,0,0 };
    int dy[] = { 0,0,-1,1 };
    int dem = 0;
    for (int k = 0; k < 4; k++) {
        int nx = x + dx[k], ny = y + dy[k];
        if (nx >= 0 && nx < 9 && ny >= 0 && ny < 9)
            if (game.luoi[nx][ny].phe == phe) dem++;
    }
    return dem;
}

bool BotViking::KiemTraCuaRa(BanCoViking& game, Phe pheCuaBot) {
    int gocX[] = { 0,0,8,8 };
    int gocY[] = { 0,8,0,8 };
    int dx[] = { -1,1,0,0 };
    int dy[] = { 0,0,-1,1 };

    int gocBiChan = 0;
    for (int g = 0; g < 4; g++) {
        bool chanDuoc = false;
        for (int k = 0; k < 4; k++) {
            int nx = gocX[g] + dx[k];
            int ny = gocY[g] + dy[k];
            if (nx >= 0 && nx < 9 && ny >= 0 && ny < 9)
                if (game.luoi[nx][ny].phe == XAM_LUOC) { chanDuoc = true; break; }
        }
        if (chanDuoc) gocBiChan++;
    }
    return gocBiChan >= 2;
}

int DemNuocDiVuaTrenBanCo(OCo luoi[9][9]) {
    int dx[] = { -1,1,0,0 };
    int dy[] = { 0,0,-1,1 };
    int dem = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (luoi[i][j].quan == VUA) {
                for (int h = 0; h < 4; h++) {
                    for (int b = 1; b < 9; b++) {
                        int nx = i + dx[h] * b, ny = j + dy[h] * b;
                        if (nx < 0 || nx >= 9 || ny < 0 || ny >= 9) break;
                        if (luoi[nx][ny].phe != TRONG) break;
                        dem++;
                    }
                }
            }
        }
    }
    return dem;
}

// Kiem tra ngay lap tuc vua co the den goc trong 1 nuoc khong
bool VuaCoTheDenGocNgay(OCo luoi[9][9]) {
    int gocX[] = { 0,0,8,8 };
    int gocY[] = { 0,8,0,8 };
    int vx = -1, vy = -1;

    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (luoi[i][j].quan == VUA) { vx = i; vy = j; }

    if (vx == -1) return false;

    for (int g = 0; g < 4; g++) {
        int gx = gocX[g], gy = gocY[g];
        // Kiem tra di thang hang
        if (vx == gx) {
            bool thong = true;
            int minY = min(vy, gy), maxY = max(vy, gy);
            for (int j = minY + 1; j < maxY; j++)
                if (luoi[vx][j].phe != TRONG) { thong = false; break; }
            if (thong) return true;
        }
        // Kiem tra di thang cot
        if (vy == gy) {
            bool thong = true;
            int minX = min(vx, gx), maxX = max(vx, gx);
            for (int i = minX + 1; i < maxX; i++)
                if (luoi[i][vy].phe != TRONG) { thong = false; break; }
            if (thong) return true;
        }
    }
    return false;
}

// Tinh so luong quan xam luoc co the tan cong vua truc tiep (de lua chon nuoc di gay ap luc)
int DemQuanXamLuocCoTheTanCongVua(OCo luoi[9][9]) {
    int vx = -1, vy = -1;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (luoi[i][j].quan == VUA) { vx = i; vy = j; }
    if (vx == -1) return 0;

    int dx[] = { -1,1,0,0 };
    int dy[] = { 0,0,-1,1 };
    int dem = 0;
    // Dem quan xam luoc dang nhin thang vua tren cung hang/cot
    for (int h = 0; h < 4; h++) {
        for (int b = 1; b < 9; b++) {
            int nx = vx + dx[h] * b, ny = vy + dy[h] * b;
            if (nx < 0 || nx >= 9 || ny < 0 || ny >= 9) break;
            if (luoi[nx][ny].phe == PHONG_THU) break;
            if (luoi[nx][ny].phe == XAM_LUOC) { dem++; break; }
        }
    }
    return dem;
}

// Ham danh gia toan dien - cai tien
float DanhGiaTrangThai(OCo luoi[9][9], int slXamLuoc, int slPhongThu, Phe pheCuaBot) {
    float diem = BoPhanPhanTich::DanhGiaTheTran(luoi, slXamLuoc, slPhongThu);

    int vx = -1, vy = -1;
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (luoi[i][j].quan == VUA) { vx = i; vy = j; }

    // --- BONUS/PENALTY rat lon: Vua co the den goc ngay lap tuc ---
    // Day la dieu kien win -> uu tien tuyet doi
    if (VuaCoTheDenGocNgay(luoi)) {
        if (pheCuaBot == PHONG_THU) return 99999.0f;  // Phong thu PHAI di
        else return -99999.0f;                          // Xam luoc PHAI ngan
    }

    int nuocDiVua = DemNuocDiVuaTrenBanCo(luoi);
    if (pheCuaBot == XAM_LUOC) diem -= nuocDiVua * 4.0f;
    else                        diem += nuocDiVua * 4.0f;

    // --- Xam luoc: thong so khoang cach den vua (bao vay) ---
    if (pheCuaBot == XAM_LUOC && vx != -1) {
        int dx[] = { -1,1,0,0 };
        int dy[] = { 0,0,-1,1 };
        // Dem quan xam luoc o gan vua (bao vay)
        int quanGanVua = 0;
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
                if (luoi[i][j].phe == XAM_LUOC) {
                    int kc = abs(i - vx) + abs(j - vy);
                    if (kc <= 3) quanGanVua++;
                }
        diem -= quanGanVua * 3.0f; // Cang nhieu quan vay vua cang tot cho xam luoc

        // Quan xam luoc gan goc chan duong thoat
        int gocX[] = { 0,0,8,8 };
        int gocY[] = { 0,8,0,8 };
        for (int g = 0; g < 4; g++) {
            for (int k = 0; k < 4; k++) {
                int nx = gocX[g] + dx[k], ny = gocY[g] + dy[k];
                if (nx >= 0 && nx < 9 && ny >= 0 && ny < 9)
                    if (luoi[nx][ny].phe == XAM_LUOC) diem -= 5.0f; // tang trong so
            }
        }

        // Ap luc truc tiep len vua: xam luoc dang tren cung hang/cot vua
        diem -= DemQuanXamLuocCoTheTanCongVua(luoi) * 6.0f;
    }

    // --- Phong thu: khoang cach vua den goc ---
    if (pheCuaBot == PHONG_THU && vx != -1) {
        int gocX[] = { 0,0,8,8 };
        int gocY[] = { 0,8,0,8 };
        int kcMinGoc = 999;
        for (int g = 0; g < 4; g++) {
            int kc = abs(vx - gocX[g]) + abs(vy - gocY[g]);
            kcMinGoc = min(kcMinGoc, kc);
        }
        diem -= kcMinGoc * 5.0f; // Vua cang gan goc cang tot cho phong thu

        // Can ve bao ve vua: can ve o gan vua la tot
        int dx[] = { -1,1,0,0 };
        int dy[] = { 0,0,-1,1 };
        int canVeGanVua = 0;
        for (int k = 0; k < 4; k++) {
            int nx = vx + dx[k], ny = vy + dy[k];
            if (nx >= 0 && nx < 9 && ny >= 0 && ny < 9)
                if (luoi[nx][ny].quan == CAN_VE) canVeGanVua++;
        }
        diem += canVeGanVua * 4.0f;
    }

    return diem;
}

// Kiem tra ngay lap tuc co the an quan doi thu (ke thit) khong
// Tra ve float bonus diem neu nuoc di nay co the an quan
float KiemTraCoTheBat(OCo luoi[9][9], int x2, int y2, Phe pheCuaBot, int slXL, int slPT) {
    float bonus = 0;
    int dx[] = { -1,1,0,0 };
    int dy[] = { 0,0,-1,1 };
    Phe pheDoiThu = (pheCuaBot == XAM_LUOC) ? PHONG_THU : XAM_LUOC;

    for (int k = 0; k < 4; k++) {
        int mx = x2 + dx[k], my = y2 + dy[k]; // vi tri co the bi an
        int bx = x2 + dx[k] * 2, by = y2 + dy[k] * 2; // vi tri phia sau
        if (mx < 0 || mx >= 9 || my < 0 || my >= 9) continue;
        if (bx < 0 || bx >= 9 || by < 0 || by >= 9) continue;

        if (luoi[mx][my].phe == pheDoiThu) {
            // Phia sau phai la dong minh hoac goc/ngai vang
            bool phiaSauOk = (luoi[bx][by].phe == pheCuaBot ||
                luoi[bx][by].laGoc || luoi[bx][by].laNgaiVang);
            if (phiaSauOk) {
                if (luoi[mx][my].quan == VUA)
                    bonus += 5000.0f; // Bat vua: uu tien tuyet doi
                else
                    bonus += 30.0f;  // An quan thuong
            }
        }
    }
    return bonus;
}

// Kiem tra nuoc di nay co bi an khong (tranh tu sap bay)
float KiemTraNguyCoBiAn(OCo luoi[9][9], int x2, int y2, Phe pheCuaBot) {
    int dx[] = { -1,1,0,0 };
    int dy[] = { 0,0,-1,1 };
    Phe pheDoiThu = (pheCuaBot == XAM_LUOC) ? PHONG_THU : XAM_LUOC;
    float penalty = 0;

    for (int k = 0; k < 4; k++) {
        int nx = x2 + dx[k], ny = y2 + dy[k];   // Doi thu o canh
        int bx = x2 - dx[k], by = y2 - dy[k];   // Phia sau minh

        if (nx < 0 || nx >= 9 || ny < 0 || ny >= 9) continue;
        if (bx < 0 || bx >= 9 || by < 0 || by >= 9) continue;

        if (luoi[nx][ny].phe == pheDoiThu) {
            bool phiaSauLaDongMinhDoiThu = (luoi[bx][by].phe == pheDoiThu ||
                luoi[bx][by].laGoc || luoi[bx][by].laNgaiVang);
            if (phiaSauLaDongMinhDoiThu) penalty -= 25.0f; // Co nguy co bi an
        }
    }
    return penalty;
}

float BotViking::DanhGiaNuocDiBot(BanCoViking& game, Phe pheCuaBot, const NuocDi& nd) {
    OCo banCoTam[9][9];
    SaoChepBanCo(game.luoi, banCoTam);

    bool flagGoc = banCoTam[nd.x2][nd.y2].laGoc;
    bool flagNgai = banCoTam[nd.x2][nd.y2].laNgaiVang;
    banCoTam[nd.x2][nd.y2] = banCoTam[nd.x1][nd.y1];
    banCoTam[nd.x2][nd.y2].laGoc = flagGoc;
    banCoTam[nd.x2][nd.y2].laNgaiVang = flagNgai;
    banCoTam[nd.x1][nd.y1] = OCo();
    banCoTam[nd.x1][nd.y1].laGoc = game.luoi[nd.x1][nd.y1].laGoc;
    banCoTam[nd.x1][nd.y1].laNgaiVang = game.luoi[nd.x1][nd.y1].laNgaiVang;

    float diem = DanhGiaTrangThai(banCoTam, game.slXamLuoc, game.slPhongThu, pheCuaBot);

    // Bonus bat quan / penalty bi an
    diem += KiemTraCoTheBat(banCoTam, nd.x2, nd.y2, pheCuaBot, game.slXamLuoc, game.slPhongThu);
    diem += KiemTraNguyCoBiAn(banCoTam, nd.x2, nd.y2, pheCuaBot);

    // Xam luoc: khong de quan le loi, uu tien tao cum
    if (pheCuaBot == XAM_LUOC) {
        int dx[] = { -1,1,0,0 };
        int dy[] = { 0,0,-1,1 };
        int dongMinh = 0;
        for (int k = 0; k < 4; k++) {
            int nx = nd.x2 + dx[k], ny = nd.y2 + dy[k];
            if (nx >= 0 && nx < 9 && ny >= 0 && ny < 9)
                if (banCoTam[nx][ny].phe == XAM_LUOC) dongMinh++;
        }
        if (dongMinh == 0) diem -= 8.0f; // phat nang hon neu le loi
        else diem += dongMinh * 3.0f;    // thuong tao cum
    }

    // Phong thu: tap hop quan bao ve vua (The Tower)
    if (pheCuaBot == PHONG_THU) {
        int dx[] = { -1,1,0,0 };
        int dy[] = { 0,0,-1,1 };
        int dongMinhKeBen = 0;
        for (int k = 0; k < 4; k++) {
            int nx = nd.x2 + dx[k], ny = nd.y2 + dy[k];
            if (nx >= 0 && nx < 9 && ny >= 0 && ny < 9)
                if (banCoTam[nx][ny].phe == PHONG_THU) dongMinhKeBen++;
        }
        diem += dongMinhKeBen * 2.0f;
    }

    return diem;
}

void SinhNuocDi(BanCoViking& game, Phe phe, vector<NuocDi>& ds) {
    int dx[] = { -1,1,0,0 };
    int dy[] = { 0,0,-1,1 };
    ds.clear();
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (game.luoi[i][j].phe == phe) {
                for (int h = 0; h < 4; h++) {
                    for (int b = 1; b < 9; b++) {
                        int nx = i + dx[h] * b, ny = j + dy[h] * b;
                        if (nx < 0 || nx >= 9 || ny < 0 || ny >= 9) break;
                        if (game.luoi[nx][ny].phe != TRONG) break;
                        if (game.DuongDiHopLe(i, j, nx, ny))
                            ds.push_back({ i,j,nx,ny });
                    }
                }
            }
        }
    }
}

NuocDi BotViking::TimNuocDiTotNhatSau(BanCoViking& game, Phe phe) {
    vector<NuocDi> ds;
    SinhNuocDi(game, phe, ds);

    if (ds.empty()) return { -1,-1,-1,-1 };

    float best = (phe == PHONG_THU) ? -9999.0f : 9999.0f;
    NuocDi bestMove = ds[0];

    for (const NuocDi& nd : ds) {
        float diem = DanhGiaNuocDiBot(game, phe, nd);
        if (phe == PHONG_THU && diem > best) { best = diem; bestMove = nd; }
        if (phe == XAM_LUOC && diem < best) { best = diem; bestMove = nd; }
    }
    return bestMove;
}

// Ham TimNuocDiTotNhat nang cap:
// - Tam nhin 2 tang (giu nguyen cu truc cu)
// - Uu tien TUYET DOI nuoc thang (vua den goc / bat vua)
// - Su dung nhieu nuoc di top-k + nhieu ngau nhien de tao su da dang
NuocDi BotViking::TimNuocDiTotNhat(BanCoViking& game, Phe pheCuaBot) {
    vector<NuocDi> ds;
    SinhNuocDi(game, pheCuaBot, ds);

    if (ds.empty()) return { -1,-1,-1,-1 };

    Phe pheDoiThu = (pheCuaBot == XAM_LUOC) ? PHONG_THU : XAM_LUOC;

    // --- KIEM TRA NUOC THANG NGAY: Vua den goc ---
    if (pheCuaBot == PHONG_THU) {
        for (const NuocDi& nd : ds) {
            if (game.luoi[nd.x1][nd.y1].quan == VUA &&
                game.luoi[nd.x2][nd.y2].laGoc) {
                return nd; // Thang ngay, di luon
            }
        }
    }

    // --- KIEM TRA NUOC THANG NGAY: Bat vua ---
    if (pheCuaBot == XAM_LUOC) {
        // Tim nuoc di co the bat vua ngay lap tuc (sau khi di xong co the kep vua)
        for (const NuocDi& nd : ds) {
            OCo banCoTam[9][9];
            SaoChepBanCo(game.luoi, banCoTam);
            bool flagGoc = banCoTam[nd.x2][nd.y2].laGoc;
            bool flagNgai = banCoTam[nd.x2][nd.y2].laNgaiVang;
            banCoTam[nd.x2][nd.y2] = banCoTam[nd.x1][nd.y1];
            banCoTam[nd.x2][nd.y2].laGoc = flagGoc;
            banCoTam[nd.x2][nd.y2].laNgaiVang = flagNgai;
            banCoTam[nd.x1][nd.y1] = OCo();

            float bonus = KiemTraCoTheBat(banCoTam, nd.x2, nd.y2, pheCuaBot,
                game.slXamLuoc, game.slPhongThu);
            if (bonus >= 5000.0f) return nd; // Bat duoc vua, di ngay
        }
    }

    // --- Danh gia tat ca nuoc di voi tam nhin 2 tang ---
    struct NuocDiVaDiem { NuocDi nd; float diem; };
    vector<NuocDiVaDiem> ketQua;

    for (const NuocDi& nd : ds) {
        OCo banCoSau1[9][9];
        SaoChepBanCo(game.luoi, banCoSau1);

        bool flagGoc1 = banCoSau1[nd.x2][nd.y2].laGoc;
        bool flagNgai1 = banCoSau1[nd.x2][nd.y2].laNgaiVang;
        bool srcGoc1 = banCoSau1[nd.x1][nd.y1].laGoc;
        bool srcNgai1 = banCoSau1[nd.x1][nd.y1].laNgaiVang;
        banCoSau1[nd.x2][nd.y2] = banCoSau1[nd.x1][nd.y1];
        banCoSau1[nd.x2][nd.y2].laGoc = flagGoc1;
        banCoSau1[nd.x2][nd.y2].laNgaiVang = flagNgai1;
        banCoSau1[nd.x1][nd.y1] = OCo();
        banCoSau1[nd.x1][nd.y1].laGoc = srcGoc1;
        banCoSau1[nd.x1][nd.y1].laNgaiVang = srcNgai1;

        OCo banCoGoc[9][9];
        SaoChepBanCo(game.luoi, banCoGoc);
        SaoChepBanCo(banCoSau1, game.luoi);

        NuocDi ndDoiThu = TimNuocDiTotNhatSau(game, pheDoiThu);

        float diemSau = 0;
        if (ndDoiThu.x1 != -1) {
            OCo banCoSau2[9][9];
            SaoChepBanCo(game.luoi, banCoSau2);

            bool flagGoc2 = banCoSau2[ndDoiThu.x2][ndDoiThu.y2].laGoc;
            bool flagNgai2 = banCoSau2[ndDoiThu.x2][ndDoiThu.y2].laNgaiVang;
            bool srcGoc2 = banCoSau2[ndDoiThu.x1][ndDoiThu.y1].laGoc;
            bool srcNgai2 = banCoSau2[ndDoiThu.x1][ndDoiThu.y1].laNgaiVang;
            banCoSau2[ndDoiThu.x2][ndDoiThu.y2] = banCoSau2[ndDoiThu.x1][ndDoiThu.y1];
            banCoSau2[ndDoiThu.x2][ndDoiThu.y2].laGoc = flagGoc2;
            banCoSau2[ndDoiThu.x2][ndDoiThu.y2].laNgaiVang = flagNgai2;
            banCoSau2[ndDoiThu.x1][ndDoiThu.y1] = OCo();
            banCoSau2[ndDoiThu.x1][ndDoiThu.y1].laGoc = srcGoc2;
            banCoSau2[ndDoiThu.x1][ndDoiThu.y1].laNgaiVang = srcNgai2;

            diemSau = DanhGiaTrangThai(banCoSau2, game.slXamLuoc, game.slPhongThu, pheCuaBot);
        }
        else {
            diemSau = (pheCuaBot == PHONG_THU) ? 9999.0f : -9999.0f;
        }

        SaoChepBanCo(banCoGoc, game.luoi);
        ketQua.push_back({ nd, diemSau });
    }

    if (ketQua.empty()) return ds[0];

    // Sap xep ket qua theo diem (tot nhat truoc)
    if (pheCuaBot == PHONG_THU) {
        sort(ketQua.begin(), ketQua.end(),
            [](const NuocDiVaDiem& a, const NuocDiVaDiem& b) { return a.diem > b.diem; });
    }
    else {
        sort(ketQua.begin(), ketQua.end(),
            [](const NuocDiVaDiem& a, const NuocDiVaDiem& b) { return a.diem < b.diem; });
    }

    // Lay top-k nuoc di tot nhat, trong do chon ngau nhien de tao su da dang
    // Dung nguong: nuoc di trong 5% diem cua nuoc tot nhat duoc xem la "tuong duong"
    float bestDiem = ketQua[0].diem;
    float nguong = abs(bestDiem) * 0.05f + 1.5f; // biên dao dong 5% + 1.5 de tranh chia 0

    vector<NuocDi> topK;
    for (auto& kq : ketQua) {
        float diff = abs(kq.diem - bestDiem);
        if (diff <= nguong) topK.push_back(kq.nd);
        if ((int)topK.size() >= 5) break; // Toi da 5 nuoc "tuong duong" de chon
    }

    // Chon ngau nhien trong top-k
    uniform_int_distribution<int> dist(0, (int)topK.size() - 1);
    int idx = dist(rng);
    return topK[idx];
}