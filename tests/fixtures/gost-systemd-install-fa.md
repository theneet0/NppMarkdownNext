# نصب سرویس‌های systemd برای GOST 3

این دو unit برای مسیرهای زیر آماده شده‌اند:

- ایران: `/usr/local/bin/ir.yaml`
- آلمان: `/usr/local/bin/de.yaml`

فایل binary باید `/usr/local/bin/gost` باشد. در کانفیگ ایران، چون گواهی‌ها با مسیر نسبی تعریف شده‌اند، فایل‌های زیر نیز باید در `/usr/local/bin` باشند:

```text
/usr/local/bin/fullchain.cer
/usr/local/bin/jojo-data.com.key
```

## دسترسی فایل‌ها

این نسخهٔ unitها عمداً با `User=root` اجرا می‌شوند. روی هر دو سرور مطمئن شو binary و کانفیگ‌ها متعلق به root و قابل‌خواندن هستند:

```bash
chown root:root /usr/local/bin/gost /usr/local/bin/ir.yaml /usr/local/bin/de.yaml
chmod 0755 /usr/local/bin/gost
chmod 0600 /usr/local/bin/ir.yaml /usr/local/bin/de.yaml
```

روی ایران:

```bash
chown root:root /usr/local/bin/fullchain.cer /usr/local/bin/jojo-data.com.key
chmod 0644 /usr/local/bin/fullchain.cer
chmod 0600 /usr/local/bin/jojo-data.com.key
```

اجرای root برای کاربرانی که همین روش را در حال حاضر استفاده می‌کنند ساده‌تر است، اما در صورت نفوذ به GOST، سطح دسترسی process کامل خواهد بود.

## نصب unitها

فایل مناسب هر سرور را در `/etc/systemd/system/` کپی کن:

```bash
# روی ایران
install -o root -g root -m 0644 gost-ir.service /etc/systemd/system/gost-ir.service

# روی آلمان
install -o root -g root -m 0644 gost-de.service /etc/systemd/system/gost-de.service
```

سپس parser و systemd را اجرا کن:

```bash
# ایران
/usr/local/bin/gost -C /usr/local/bin/ir.yaml -O yaml >/dev/null
systemctl daemon-reload
systemctl enable --now gost-ir.service

# آلمان
/usr/local/bin/gost -C /usr/local/bin/de.yaml -O yaml >/dev/null
systemctl daemon-reload
systemctl enable --now gost-de.service
```

## مشاهده و عیب‌یابی

```bash
systemctl status gost-ir.service --no-pager
systemctl status gost-de.service --no-pager
journalctl -u gost-ir.service -f -o cat
journalctl -u gost-de.service -f -o cat
```

برای بررسی سقف فایل‌ها:

```bash
systemctl show gost-ir.service -p LimitNOFILE -p TasksMax
systemctl show gost-de.service -p LimitNOFILE -p TasksMax
```

این unitها GOST را foreground اجرا می‌کنند؛ بنابراین از `-D` یا daemonize کردن داخل systemd استفاده نکن.


## مدیریت سرویس (Stop, Restart, Disable)

### ۱. راه‌اندازی مجدد (Restart)
برای اعمال تغییرات در فایل کانفیگ پس از ویرایش، سرویس را ری‌استارت کن:

```bash
# ایران
systemctl restart gost-ir.service

# آلمان
systemctl restart gost-de.service
```

### ۲. متوقف کردن سرویس (Stop)
برای قطع موقت کارکرد بدون تغییر در وضعیت بوت سیستم:

```bash
# ایران
systemctl stop gost-ir.service

# آلمان
systemctl stop gost-de.service
```

### ۳. غیرفعال‌سازی (Disable)
برای جلوگیری از اجرای خودکار در زمان بالا آمدن سرور (بوت شدن):

```bash
# ایران (فقط حذف از بوت)
systemctl disable gost-ir.service

# آلمان (فقط حذف از بوت)
systemctl disable gost-de.service
```

> **نکته:** در صورتی که می‌خواهی سرویس را همزمان **متوقف** و **از استارتاپ خارج** کنی، از فلگ `--now` استفاده کن:
> ```bash
> # توقف آنی + غیرفعال‌سازی بوت در ایران
> systemctl disable --now gost-ir.service
> 
> # توقف آنی + غیرفعال‌سازی بوت در آلمان
> systemctl disable --now gost-de.service
> ```

---

## مشاهده وضعیت و لاگ‌های زنده (Live Logs)

### مشاهده لاگ‌های زنده (Stream / Follow)
جهت مانیتورینگ لحظه‌ای، رهگیری اتصالات و دیباگ خطاهای احتمالی:

```bash
# ایران: ۵۰ خط آخر + استریم لاگ‌های جدید با خروجی بدون بافر
journalctl -u gost-ir.service -f -n 50 -o cat

# آلمان: ۵۰ خط آخر + استریم لاگ‌های جدید با خروجی بدون بافر
journalctl -u gost-de.service -f -n 50 -o cat
```

اگر نیاز به ثبت تایم‌استمپ کامل دارید:

```bash
# مشاهده لاگ زنده همراه با تاریخ و ساعت استاندارد ISO
journalctl -u gost-ir.service -f --output=short-iso
journalctl -u gost-de.service -f --output=short-iso
```

### بررسی وضعیت کنونی (Status)
```bash
# ایران
systemctl status gost-ir.service --no-pager

# آلمان
systemctl status gost-de.service --no-pager
```

---

## عیب‌یابی منابع و محدودیت‌ها

برای بررسی سقف تعداد فایل‌های باز (`LimitNOFILE`) و تعداد تسک‌های مجاز:

```bash
# ایران
systemctl show gost-ir.service -p LimitNOFILE -p TasksMax

# آلمان
systemctl show gost-de.service -p LimitNOFILE -p TasksMax
```

> **مهم:** این unitها GOST را در حالت foreground اجرا می‌کنند؛ بنابراین از سوییچ `-D` یا رفتن به پس‌زمینه (daemonize) داخل دستورات systemd اکیداً خودداری شود.
