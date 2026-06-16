#include <stdio.h>
#include <string.h>
#include <time.h> 

#define MAX_MENU 5
#define MAX_PESANAN 100
#define MAX_TRANSAKSI 100

// Struct menu untuk menyimpan data menu restoran
typedef struct
{
    int id;
    char nama[30];
    int harga;
} Menu;

// Struct pesanan untuk menyimpan data pesanan pelanggan
typedef struct
{
    char nama_menu[30];
    int harga_satuan;
    int jumlah;
    int subtotal;
} Pesanan;

// Struct riwayat transaksi untuk menyimpan data transaksi
typedef struct
{
    int id_transaksi;
    int total_belanja;
    int diskon;
    int total_akhir;
} RiwayatTransaksi;

// Deklarasi Fungsi
void tampilkanMenu(Menu list[], int n);
void urutkanMenuTermurah(Menu list[], int n);
void hitungDiskon(int totalBelanja, int isMember, int *diskon, int *totalAkhir);
void cetakTanggal(); 

int main()
{
    Menu daftarMenu[MAX_MENU] = {
        {1, "Nasi Goreng Spesial", 25000},
        {2, "Mie Ayam Pangsit", 18000},
        {3, "Ayam Bakar Taliwang", 30000},
        {4, "Es Teh Manis", 5000},
        {5, "Jus Alpukat", 12000}};

    int menuSistem;
    int totalPendapatanHarian = 0;
    int jumlahPelangganHarian = 0;
    
    // Array untuk menyimpan history Laporan Harian
    RiwayatTransaksi laporanHarian[MAX_TRANSAKSI];

    printf("=== SELAMAT DATANG DI SISTEM RESTORAN DIGITAL ===\n");

    // Loop Sistem Utama (Kasir)
    do
    {
        printf("\n===============================================\n");
        printf("                MENU SISTEM KASIR              \n");
        printf("===============================================\n");
        printf("1. Mulai Transaksi\n");
        printf("2. Tutup Kasir & Lihat Laporan Harian\n");
        printf("Pilih opsi (1-2): ");
        if (scanf("%d", &menuSistem) != 1)
        {
            while (getchar() != '\n')
                ;

            printf("[Error] Input tidak valid! Harap masukkan angka.\n");
            menuSistem = 0; 
            continue;       
        }

        if (menuSistem == 1)
        {
            Pesanan keranjang[MAX_PESANAN];
            int jumlahJenisPesanan = 0;
            int pilihan, idMenu, qty, i;
            int totalBelanja = 0;
            int statusMember = 0;
            int diskon = 0;
            int totalAkhir = 0;
            int uangBayar = 0;

            printf("\n--- TRANSAKSI (TRX-%04d) ---\n", jumlahPelangganHarian + 1);
            printf("Apakah pelanggan memiliki kartu member? (1 = Ya, 0 = Tidak): ");
            if (scanf("%d", &statusMember) != 1 || (statusMember != 0 && statusMember != 1))
            {
                while (getchar() != '\n')
                    ;
                printf("[Error] Input tidak valid! Anggap sebagai non-member.\n");
                statusMember = 0;
            }

            // Loop Transaksi Pemesanan
            do
            {
                printf("\n--- MENU PEMESANAN ---\n");
                printf("1. Lihat Menu (Default)\n");
                printf("2. Urutkan Menu (Termurah -> Termahal)\n");
                printf("3. Tambah Pesanan ke Keranjang\n");
                printf("4. Selesai & Cetak Struk Pemesanan\n");
                printf("5. Batalkan Transaksi\n");
                printf("Pilih opsi (1-5): ");
                if (scanf("%d", &pilihan) != 1)
                {
                    while (getchar() != '\n')
                        ; 
                    printf("[Error] Input tidak valid! Harap masukkan angka.\n");
                    pilihan = 0;
                    continue;
                }

                switch (pilihan)
                {
                case 1:
                    tampilkanMenu(daftarMenu, MAX_MENU);
                    break;

                case 2:
                    urutkanMenuTermurah(daftarMenu, MAX_MENU);
                    printf("\n[Sukses] Menu berhasil diurutkan!\n");
                    tampilkanMenu(daftarMenu, MAX_MENU);
                    break;

                case 3:
                    tampilkanMenu(daftarMenu, MAX_MENU);
                    printf("\nMasukkan ID Menu yang ingin dipesan: ");
                    scanf("%d", &idMenu);

                    if (idMenu < 1 || idMenu > MAX_MENU)
                    {
                        printf("[Error] ID Menu tidak valid!\n");
                        break;
                    }

                    printf("Masukkan Jumlah (Qty): ");
                    scanf("%d", &qty);
                    if (qty <= 0)
                    {
                        printf("[Error] Jumlah harus lebih dari 0!\n");
                        break;
                    }

                    for (i = 0; i < MAX_MENU; i++)
                    {
                        if (daftarMenu[i].id == idMenu)
                        {
                            strcpy(keranjang[jumlahJenisPesanan].nama_menu, daftarMenu[i].nama);
                            keranjang[jumlahJenisPesanan].harga_satuan = daftarMenu[i].harga;
                            keranjang[jumlahJenisPesanan].jumlah = qty;
                            keranjang[jumlahJenisPesanan].subtotal = daftarMenu[i].harga * qty;
                            totalBelanja += keranjang[jumlahJenisPesanan].subtotal;
                            jumlahJenisPesanan++;
                            printf("[Sukses] %d %s berhasil ditambahkan.\n", qty, daftarMenu[i].nama);
                            break;
                        }
                    }
                    break;

                case 4:
                    if (jumlahJenisPesanan == 0)
                    {
                        printf("\n[Peringatan] Keranjang masih kosong! Batalkan transaksi? (Pilih 5 untuk batal, atau 1-3 untuk lanjut)\n");
                        pilihan = 0; 
                    }
                    else
                    {
                        printf("\nMemproses struk pembayaran...\n");
                    }
                    break;

                case 5:
                    printf("\n[Info] Transaksi telah dibatalkan.\n");
                    break;

                default:
                    printf("Pilihan tidak tersedia!\n");
                }
            } while (pilihan != 4 && pilihan != 5);

            if (pilihan == 5)
            {
                continue;
            }

            // Proses Pembayaran Pelanggan
            hitungDiskon(totalBelanja, statusMember, &diskon, &totalAkhir);

            // --- PENCETAKAN STRUK DENGAN ID TRANSAKSI ---
            printf("\n======================================================\n");
            printf("                 STRUK PEMBAYARAN                     \n");
            printf("======================================================\n");
            cetakTanggal();
            printf("ID Transaksi    : TRX-%04d\n", jumlahPelangganHarian + 1);
            printf("------------------------------------------------------\n");
            printf("%-3s | %-22s | %-5s | %-10s\n", "No", "Nama Menu", "Qty", "Subtotal");
            printf("------------------------------------------------------\n");

            for (i = 0; i < jumlahJenisPesanan; i++)
            {
                printf("%-3d | %-22s | %-5d | Rp%10d\n",
                       i + 1,
                       keranjang[i].nama_menu,
                       keranjang[i].jumlah,
                       keranjang[i].subtotal);
            }

            printf("======================================================\n");
            printf("Total Belanja   : Rp%10d\n", totalBelanja);
            printf("Potongan Diskon : Rp%10d\n", diskon);
            printf("Total Akhir     : Rp%10d\n", totalAkhir);
            printf("======================================================\n");

            while (1)
            {
                printf("Masukkan Nominal Pembayaran: Rp");
                if (scanf("%d", &uangBayar) != 1)
                {
                    while (getchar() != '\n')
                        ;

                    printf("[Error] Input tidak valid! Harap masukkan angka.\n");
                    continue; 
                }

                if (uangBayar >= totalAkhir)
                {
                    int kembalian = uangBayar - totalAkhir;
                    if (kembalian > 0)
                    {
                        printf("Pembayaran Sukses! Kembalian Anda: Rp%d\n", kembalian);
                    }
                    else
                    {
                        printf("Pembayaran Sukses!\n");
                    }

                    // Menyimpan data transaksi ke array Riwayat Harian menggunakan id_transaksi
                    laporanHarian[jumlahPelangganHarian].id_transaksi = jumlahPelangganHarian + 1;
                    laporanHarian[jumlahPelangganHarian].total_belanja = totalBelanja;
                    laporanHarian[jumlahPelangganHarian].diskon = diskon;
                    laporanHarian[jumlahPelangganHarian].total_akhir = totalAkhir;

                    // Tambahkan ke rekap harian setelah sukses dibayar
                    totalPendapatanHarian += totalAkhir;
                    jumlahPelangganHarian++;
                    break;
                }
                else
                {
                    printf("[Gagal] Uang kurang sebesar: Rp%d. Silakan coba lagi.\n", totalAkhir - uangBayar);
                }
            }

            printf("\n======================================================\n");
            printf("          Terima Kasih Telah Berkunjung!              \n");
            printf("======================================================\n");
        }
        else if (menuSistem == 2)
        {
            // --- LAPORAN HARIAN DENGAN FORMAT ID TRANSAKSI ---
            printf("\n======================================================\n");
            printf("               LAPORAN TRANSAKSI HARIAN               \n");
            printf("======================================================\n");
            cetakTanggal();
            
            if (jumlahPelangganHarian == 0) {
                printf("\nBelum ada transaksi yang terjadi hari ini.\n\n");
            } else {
                printf("------------------------------------------------------\n");
                printf("%-12s | %-13s | %-9s | %-11s\n", "ID Transaksi", "Total Belanja", "Diskon", "Total Bayar");
                printf("------------------------------------------------------\n");
                for (int i = 0; i < jumlahPelangganHarian; i++)
                {
                    printf("TRX-%04d     | Rp%-11d | Rp%-7d | Rp%-9d\n",
                           laporanHarian[i].id_transaksi,
                           laporanHarian[i].total_belanja,
                           laporanHarian[i].diskon,
                           laporanHarian[i].total_akhir);
                }
                printf("------------------------------------------------------\n");
            }
            
            printf("Total Transaksi           : %d Transaksi\n", jumlahPelangganHarian);
            printf("Total Pendapatan Hari Ini : Rp%d\n", totalPendapatanHarian);
            printf("======================================================\n");
            printf("Sistem Kasir Ditutup. Selamat beristirahat!\n");
        }
        else
        {
            printf("[Error] Pilihan sistem tidak valid!\n");
        }

    } while (menuSistem != 2);

    return 0;
}

// Implementasi Fungsi
void tampilkanMenu(Menu list[], int n)
{
    printf("\n=========================================\n");
    printf("%-4s | %-22s | %-10s\n", "ID", "Nama Menu", "Harga");
    printf("-----------------------------------------\n");
    for (int i = 0; i < n; i++)
    {
        printf("%-4d | %-22s | Rp%10d\n", list[i].id, list[i].nama, list[i].harga);
    }
    printf("=========================================\n");
}

void urutkanMenuTermurah(Menu list[], int n)
{
    int i, j;
    Menu temp;
    for (i = 0; i < n - 1; i++)
    {
        for (j = 0; j < n - i - 1; j++)
        {
            if (list[j].harga > list[j + 1].harga)
            {
                temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
        }
    }
}

void hitungDiskon(int totalBelanja, int isMember, int *diskon, int *totalAkhir)
{
    if (isMember == 1)
    {
        if (totalBelanja >= 100000)
        {
            *diskon = totalBelanja * 0.20;
        }
        else if (totalBelanja >= 50000)
        {
            *diskon = totalBelanja * 0.10;
        }
        else
        {
            *diskon = totalBelanja * 0.05;
        }
    }
    else
    {
        if (totalBelanja >= 100000)
        {
            *diskon = totalBelanja * 0.10;
        }
        else if (totalBelanja >= 50000)
        {
            *diskon = totalBelanja * 0.05;
        }
        else
        {
            *diskon = 0;
        }
    }
    *totalAkhir = totalBelanja - *diskon;
}

void cetakTanggal()
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("Tanggal Transaksi: %02d-%02d-%d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}