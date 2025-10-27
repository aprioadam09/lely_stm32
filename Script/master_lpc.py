import canopen
import time
import sys
import select

def notifikasi_tombol_ditekan(message):
    """Callback yang akan dipanggil saat TPDO status tombol diterima."""
    for var in message:
        # var.name akan berisi nama objek dari file EDS, misal: 
        # "Button Object with custom SDO upload callback"
        # Kita cek apakah objek yang dimaksud ada di dalam pesan ini
        if var.index == 0x2110:
            status = "DITEKAN" if var.raw == 1 else "DILEPAS"
            print(f"\nNOTIFIKASI: Tombol pada slave sekarang dalam keadaan {status} (Nilai: {var.raw})")
            print("> ", end="", flush=True) # Mencetak ulang prompt

# Node ID slave LPC1769 Anda
NODE_ID = 2

# Buat network
network = canopen.Network()
network.connect(bustype='socketcan', channel='can0')

print("Mencari node...")
try:
    # Penting: Pastikan file EDS-mu sudah lengkap dengan objek yang relevan
    node = network.add_node(NODE_ID, 'lpc17xx.dcf') 
    print(f"Slave dengan Node ID {NODE_ID} ditemukan.")
except Exception as e:
    print(f"Error saat menambahkan node: {e}")
    network.disconnect()
    exit()

# Setel NMT state ke OPERATIONAL
print("Mengubah state node ke OPERATIONAL...")
node.nmt.state = 'OPERATIONAL'
time.sleep(1) # Beri waktu slave untuk siap

# network.sync.start(5.0)

print("Mendaftarkan callback untuk notifikasi tombol (TPDO)...")
# Kita subscribe ke TPDO pertama dari slave (sesuai konfigurasi 0x1A00 di sdev.c)
node.tpdo[1].add_callback(notifikasi_tombol_ditekan)

print("Membaca konfigurasi TPDO dari slave untuk sinkronisasi...")
node.tpdo.read()

print("\n--- Master Siap ---")
print("Node dalam keadaan OPERATIONAL.")
print("Perintah yang tersedia:")
print("  - 'led [0-3]'      : Mengontrol LED via RPDO.")
print("  - 'baca_led'       : Membaca status LED via SDO.")
print("  - 'tulis_valid'    : Menulis nilai 42 ke obj 0x2000 (akan berhasil).")
print("  - 'tulis_gagal'    : Menulis nilai 10 ke obj 0x2000 (akan gagal).")
print("  - 'baca_custom'    : Membaca dari obj 0x2001 (selalu mengembalikan 42).")
print("  - 'keluar'         : Menghentikan program.")
print("----------------------------------------------------")

led_control_cob_id = 0x200 + NODE_ID
prompt_printed = True
print("> ", end="", flush=True)

try:
    while True:
        # Cek apakah ada input dari keyboard, dengan timeout 0.1 detik
        rlist, _, _ = select.select([sys.stdin], [], [], 0.1)

        if rlist:
            # Jika ada input, baca satu baris
            perintah = sys.stdin.readline().strip().lower()

            if perintah == 'keluar':
                break

            elif perintah.startswith('led'):
                try:
                    parts = perintah.split()
                    if len(parts) > 1 and parts[1].isdigit():
                        value = int(parts[1])
                        if 0 <= value <= 7:
                            # 1. Kirim Perintah RPDO
                            print(f"-> Mengirim RPDO ke COB-ID {hex(led_control_cob_id)} dengan nilai: {value}")
                            network.send_message(led_control_cob_id, [value])
                        else:
                            print("-> Nilai tidak valid. Gunakan 0, 1, 2, atau 3.")
                    else:
                        print("-> Format salah. Gunakan: led [angka]")
                except Exception as e:
                    print(f"-> Gagal mengirim RPDO: {e}")
            
            # --- PERINTAH BARU: BACA STATUS LED VIA SDO ---
            elif perintah == 'baca_led':
                try:
                    led_status = node.sdo[0x2100].raw
                    print(f"-> SDO Read: Status LED (0x2100) saat ini adalah: {led_status}")
                except Exception as e:
                    print(f"-> GAGAL SDO Read: {e}")

            # --- PERINTAH BARU: TULIS NILAI VALID KE 0x2000 ---
            elif perintah == 'tulis_valid':
                try:
                    print("-> SDO Write: Mencoba menulis nilai 42 ke objek 0x2000...")
                    # .raw akan melakukan SDO download
                    node.sdo[0x2000].raw = 42
                    print("   -> SUKSES! Slave menerima nilai tersebut.")
                except canopen.sdo.SdoAbortedError as e:
                    print(f"   -> GAGAL! Slave menolak nilai: {e}")

            # --- PERINTAH BARU: TULIS NILAI GAGAL KE 0x2000 ---
            elif perintah == 'tulis_gagal':
                try:
                    print("-> SDO Write: Mencoba menulis nilai 10 ke objek 0x2000...")
                    node.sdo[0x2000].raw = 10
                    print("   -> SUKSES! Slave menerima nilai tersebut.")
                except canopen.sdo.SdoAbortedError as e:
                    # Kita mengharapkan error ini terjadi
                    print(f"   -> GAGAL SEPERTI DIHARAPKAN! Slave menolak nilai: {e}")
                    print("   -> (Ini membuktikan callback validasi di slave berjalan)")

            # --- PERINTAH BARU: BACA DARI OBJEK CUSTOM 0x2001 ---
            elif perintah == 'baca_custom':
                try:
                    # .raw akan melakukan SDO upload
                    val = node.sdo[0x2001].raw
                    print(f"-> SDO Read: Membaca dari objek 0x2001, nilai yang dikembalikan: {val}")
                    print("   -> (Ini membuktikan callback upload di slave berjalan)")
                except Exception as e:
                    print(f"-> GAGAL SDO Read: {e}")

            # elif perintah == 'reset' :
            #     try:
            #         print("-> Mengirim perintah NMT: Reset Node...")
            #         node.nmt.send_command(0x81) # Reset Node
            #     except Exception as e:
            #         print(f"-> Gagal mengirim perintah NMT: {e}")

            elif perintah: # Jika ada input tapi bukan perintah yang dikenal
                print(f"-> Perintah '{perintah}' tidak dikenal.")
            
            prompt_printed = False
            
        # Cetak ulang prompt jika belum tercetak
        if not prompt_printed:
            print("> ", end="", flush=True)
            prompt_printed = True
        
        # Beri sedikit waktu agar CPU tidak bekerja 100%
        time.sleep(0.05)

except KeyboardInterrupt:
    pass
finally:
    print("\nMereset komunikasi node dan memutuskan koneksi...")
    # network.sync.stop()
    node.nmt.state = 'RESET COMMUNICATION'
    network.disconnect()
    print("Selesai.")