#include "Viking.h"
int main() {
    BanCoViking game;
    ChuTro admin;
    BotViking bot;
    int doKho = admin.ThietLapLuatChoi();
    game.KhoiTao(doKho);
    cout << "\n=== CHON CHE DO CHOI ===\n";
    cout << "1. Nguoi vs Nguoi\n";
    cout << "2. Nguoi (Phong Thu - Vua) vs Bot (Xam Luoc)\n";
    cout << "3. Nguoi (Xam Luoc) vs Bot (Phong Thu - Vua)\n";
    cout << "4. Bot vs Bot (Demo)\n";
    cout << "Chon (1/2/3/4): ";
    int cheDo;
    cin >> cheDo;
    int choPhepBotDi = 0;
    while (!game.DaKetThuc()) {
        game.VeBanCo();
        if (cheDo == 4) {
            cout << "\nNhap 1 de bat dau / tiep tuc: ";
            cin >> choPhepBotDi;
        }
        if ((cheDo == 2 && game.layLuotDi() == XAM_LUOC) ||
            (cheDo == 3 && game.layLuotDi() == PHONG_THU) ||
            (cheDo == 4 && choPhepBotDi == 1)) {
            cout << "\n[BOT] Dang suy nghi...\n";
            NuocDi nd = bot.TimNuocDiTotNhat(game, game.layLuotDi());
            if (nd.x1 != -1) {
                cout << "Bot danh: " << nd.x1 << " " << nd.y1 << " -> " << nd.x2 << " " << nd.y2 << "\n";
                game.ThucHienNuocDi(nd.x1, nd.y1, nd.x2, nd.y2);
            }
            else {
                cout << "Bot het nuoc di hop le!\n";
                break;
            }
        }
        else {
            int x1, y1, x2, y2;
            cout << "\nNhap toa do di chuyen (xViTri yVitri xDich yDich): ";
            cin >> x1 >> y1 >> x2 >> y2;
            game.ThucHienNuocDi(x1, y1, x2, y2);
        }
    }
    cout << "TRO CHOI KET THUC. CAM ON DA CHOI!" << endl;
    return 0;
}