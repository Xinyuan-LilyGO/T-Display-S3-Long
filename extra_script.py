Import('env')
import os
import shutil

# --- 请在此处修改为你自己的文件夹名称 ---
TARGET_DIR = "firmware_bin"
# ------------------------------------

def copy_firmware_files(source, target, env):
    """
    编译完成后，将生成的 .bin 文件复制到目标文件夹。
    """
    print(">>> [Extra Script] 开始复制固件文件...")
    
    # 确保目标文件夹存在
    if not os.path.exists(TARGET_DIR):
        os.makedirs(TARGET_DIR)
        print(f">>> [Extra Script] 已创建目标文件夹: {TARGET_DIR}")

    # 确定编译环境的构建目录
    build_dir = env.subst("$BUILD_DIR")
    copied_count = 0
    
    # 遍历构建目录，查找并复制 .bin 文件
    for root, dirs, files in os.walk(build_dir):
        for file in files:
            if file.endswith(".bin"):
                source_path = os.path.join(root, file)
                dest_path = os.path.join(TARGET_DIR, file)
                try:
                    shutil.copy2(source_path, dest_path)
                    print(f">>> [Extra Script] 已复制: {file}")
                    copied_count += 1
                except Exception as e:
                    print(f">>> [Extra Script] 复制 {file} 时出错: {e}")

    print(f">>> [Extra Script] 复制完成，共处理 {copied_count} 个文件。")

# 注册构建后动作：当 'buildprog' 任务完成时，执行 copy_firmware_files 函数
env.AddPostAction("buildprog", copy_firmware_files)