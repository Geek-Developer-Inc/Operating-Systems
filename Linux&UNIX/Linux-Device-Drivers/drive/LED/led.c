/*        source /etc/profile    source ~/.bashrc    */
#define GPM4CON (*(volatile unsigned int*)0x110002E0)
#define GPM4DAT (*(volatile unsigned int*)0x110002E4)
/*      两个嵌套的for循环实现延时     */
void delay_nms(unsigned int ms)
{
	unsigned int i = 0, j = 0;
	for (i = ms; i > 0; i--)
	{
		for (j = 123;j > 0; j--)
		{
			;
		}
	}
}

int main(void)
{
	/*   LED初始化   */
	GPM4CON &= ~((0x0f << 3 * 4) | (0x0f << 2 * 4) | (0x0f << 1 * 4) | (0x0f << 0 * 4));
	GPM4CON &= ~((0x01 << 3 * 4) | (0x01 << 2 * 4) | (0x01 << 1 * 4) | (0x01 << 0 * 4));
	int i = 0,falge=0;
	do
	{
		GPM4DAT |= ((0x01 << 3 * 4) | (0x01 << 2 * 4) | (0x01 << 1 * 4) | (0x01 << 0 * 4));
		delay_nms(200);
		GPM4DAT &= ~((0x01 << 3 * 4) | (0x01 << 2 * 4) | (0x01 << 1 * 4) | (0x01 << 0 * 4));
		delay_nms(200);
	} while (1);
	return 0;
}
