## 本实验基于yolov5进行目标识别
### conda activate THZ
python完整代码
``` python
import matplotlib.pyplot as plt
import pymysql
import argparse
import time
import datetime
import os
import base64
import argparse
import os
import platform
import sys
import torch

from pathlib import Path
from DBUtils.PooledDB import PooledDB
from scipy.interpolate import griddata
from PIL import Image
from io import BytesIO
from paho.mqtt import client as mqtt
from models.common import DetectMultiBackend
from utils.dataloaders import IMG_FORMATS, VID_FORMATS, LoadImages, LoadScreenshots, LoadStreams
from utils.general import (LOGGER, Profile, check_file, check_img_size, check_imshow, check_requirements, colorstr, cv2,
                           increment_path, non_max_suppression, print_args, scale_boxes, strip_optimizer, xyxy2xywh)
from utils.plots import Annotator, colors, save_one_box
from utils.torch_utils import select_device, smart_inference_mode

DBNAME = 'THZ'
DBTABLENAME = 'thz'
LastProcessTime = datetime.datetime(2020,1,1,0,0,0)
findLastChangeTime = "SELECT UPDATE_TIME FROM information_schema.tables WHERE TABLE_SCHEMA = '" + DBNAME + "' AND TABLE_NAME = '" + DBTABLENAME + "'"
findData = "SELECT * FROM " + DBNAME + "." + DBTABLENAME
insertPicture = "INSERT INTO my_images (time, c_pic, c_test) VALUES (%s, %s, %s)"
insertResult = "INSERT INTO my_images (c_test) VALUES (%s)"
broker = '144.24.71.140'
port = 1883
topic = "pythonstart"
client_id = "pythonweb"


parser = argparse.ArgumentParser(description="mysql")
parser.add_argument("-u","--user",type=str,help="MsSQL Username",default = 'root')
parser.add_argument("-p","--password",type=str,help="MsSQL Password",default='Wsad080874')
parser.add_argument("-i","--hostip",type=str,help="remote/local ip",default='localhost')
parser.add_argument("-o","--port",type=int,help="MySQL port",default=3306)

i = 1


args = parser.parse_args()

DBUSER = args.user
DBPASS = args.password
DBHOST = args.hostip
DBPORT = args.port


pool = PooledDB(
    creator = pymysql,
    host = DBHOST,
    user = DBUSER,
    password = DBPASS,
    database = DBNAME,
    port = DBPORT
)

FILE = Path(__file__).resolve()
ROOT = FILE.parents[0]  # YOLOv5 root directory
if str(ROOT) not in sys.path:
    sys.path.append(str(ROOT))  # add ROOT to PATH
ROOT = Path(os.path.relpath(ROOT, Path.cwd()))  # relative



def connect_mqtt():
    print("Python start")
    client = mqtt.Client()
    client.connect(broker, port)
    client.subscribe(topic)
    # client.loop_start()  # 启动循环接收消息
    return client

def subscribe(client: mqtt):
    def on_message(client, userdata, msg):
        if msg.topic == topic and msg.payload.decode() == 'start':
            conn, cursor = connectServer()
            poll(conn, cursor)
            print("ssss")
            conn.close()

    client.subscribe(topic)
    client.on_message = on_message

def connectServer():
    try:
        conn = pool.connection()
        cursor = conn.cursor()
        print("Connect to :",DBHOST,"Success.")
        return conn,cursor
    except Exception as e:
        print("Failed to connect database: ", e)
        exit()

def poll(conn, cursor):
    w=1
    global LastProcessTime
    savenum = 0
    # while True:
    cursor.execute(findLastChangeTime)
    gettime = cursor.fetchall()
    if gettime is None:
        print("Not find change time! Something Wrong?")
        # break
    LastChangeTime = gettime[0][0]
    print("Get Table Last Change Time :",LastChangeTime)
    # if LastProcessTime >= LastChangeTime:
    if w!=1:
            #wait for change
        time.sleep(10)
        # continue
    else:
            #find data
        print("Find New Data!\nLastProcessTime is :",LastProcessTime,"    LastChangeTime is :",LastChangeTime)
            #Make sure to write complete
        time.sleep(5)
        x = []
        y = []
        z = []
            
        cursor.execute(findData)
        sqlData = cursor.fetchall()
        for i in range(len(sqlData)):
            if (sqlData[i][0] == None or sqlData[i][1] == None or sqlData[i][2] == None):
                print("SQLData have none value, wtf!")
                break
            x.append(-float(sqlData[i][0]))
            y.append(-float(sqlData[i][1]))
            z.append(-float(sqlData[i][2]))
                #print('x: %f, y: %f, z: %f' %(x[i], y[i], z[i]))
        print("Check data done,now process it.")
        try:
            plt.scatter(x, y, c=z, cmap='viridis')
            filename = str(datetime.datetime.now()) + ".png"
            path = "/root/SaveFile/" + filename
            print("Now time is: ",filename,"  Saving file to :", path)
            plt.savefig(path)
        except Exception as e:
            print("Process data failed: ", e)
        savenum+=1
        print("Save to local succuss,now insert to database")
            # Read image data
        try:
            with open(path, 'rb') as f:
                img_data = f.read()
                    # Encode image data as base64 string
                img_base64 = base64.b64encode(img_data).decode('utf-8')
                    # Insert image data into database
                # cursor.execute(insertPicture, (filename,  img_base64))
                # conn.commit()
        except Exception as e:
            print("Insert to database failed: ", e)
            # break
        LastProcessTime = datetime.datetime.now()
        print("Insert to database Success! Update LastProcessTime to : %s This is %dth time to save file."%(LastProcessTime, savenum))

        print("Start test")
        opt = parse_opt()
        main(opt)
        files = os.listdir("/root/AItest/exp/")
        png_files = []
        for file in files:
            if file.endswith(".png"):
                png_files.append(file)

            for png in png_files:
                path = "/root/AItest/exp/" + png
                with open(path, 'rb') as p:
                    img_da = p.read()
                    # Encode image data as base64 string
                    img_base = base64.b64encode(img_da).decode('utf-8')
                    # Insert image data into database
                    cursor.execute(insertPicture, (filename, img_base64, img_base))
                    print("Success test")
                    conn.commit()

# def test(conn, cursor):
#     files = os.listdir("/root/AItest/exp/")
#     png_files = []
#     for file in files:
#         if file.endswith(".png"):
#             png_files.append(file)
#
#         for png in png_files:
#             id = 1
#             path = "/root/AItest/exp/" + png
#             with open(path, 'rb') as p:
#                 img_da = p.read()
#         # Encode image data as base64 string
#                 img_base64 = base64.b64encode(img_da).decode('utf-8')
#         # Insert image data into database
#                 cursor.execute(insertResult, (id, img_base64))
#                 print("Success test")
#                 conn.commit()


@smart_inference_mode()
def run(
        weights= '/root/data/best.pt',  # model path or triton URL
        source= '/root/SaveFile',  # file/dir/URL/glob/screen/0(webcam)
        data= '/root/data/data.yaml',  # dataset.yaml path
        imgsz=(480, 640),  # inference size (height, width)
        conf_thres=0.25,  # confidence threshold
        iou_thres=0.45,  # NMS IOU threshold
        max_det=1000,  # maximum detections per image
        device='',  # cuda device, i.e. 0 or 0,1,2,3 or cpu
        view_img=False,  # show results
        save_txt=False,  # save results to *.txt
        save_conf=False,  # save confidences in --save-txt labels
        save_crop=False,  # save cropped prediction boxes
        nosave=False,  # do not save images/videos
        classes=None,  # filter by class: --class 0, or --class 0 2 3
        agnostic_nms=False,  # class-agnostic NMS
        augment=False,  # augmented inference
        visualize=False,  # visualize features
        update=False,  # update all models
        project=ROOT / 'runs/detect',  # save results to project/name
        name='exp',  # save results to project/name
        exist_ok=False,  # existing project/name ok, do not increment
        line_thickness=3,  # bounding box thickness (pixels)
        hide_labels=False,  # hide labels
        hide_conf=False,  # hide confidences
        half=False,  # use FP16 half-precision inference
        dnn=False,  # use OpenCV DNN for ONNX inference
        vid_stride=1,  # video frame-rate stride
):
    source = str(source)
    save_img = not nosave and not source.endswith('.txt')  # save inference images
    is_file = Path(source).suffix[1:] in (IMG_FORMATS + VID_FORMATS)
    is_url = source.lower().startswith(('rtsp://', 'rtmp://', 'http://', 'https://'))
    webcam = source.isnumeric() or source.endswith('.txt') or (is_url and not is_file)
    screenshot = source.lower().startswith('screen')
    if is_url and is_file:
        source = check_file(source)  # download

    # Directories
    save_dir = increment_path(Path(project) / name, exist_ok=exist_ok)  # increment run
    (save_dir / 'labels' if save_txt else save_dir).mkdir(parents=True, exist_ok=True)  # make dir

    # Load model
    device = select_device(device)
    model = DetectMultiBackend(weights, device=device, dnn=dnn, data=data, fp16=half)
    stride, names, pt = model.stride, model.names, model.pt
    imgsz = check_img_size(imgsz, s=stride)  # check image size

    # Dataloader
    bs = 1  # batch_size
    if webcam:
        view_img = check_imshow(warn=True)
        dataset = LoadStreams(source, img_size=imgsz, stride=stride, auto=pt, vid_stride=vid_stride)
        bs = len(dataset)
    elif screenshot:
        dataset = LoadScreenshots(source, img_size=imgsz, stride=stride, auto=pt)
    else:
        dataset = LoadImages(source, img_size=imgsz, stride=stride, auto=pt, vid_stride=vid_stride)
    vid_path, vid_writer = [None] * bs, [None] * bs

    # Run inference
    model.warmup(imgsz=(1 if pt or model.triton else bs, 3, *imgsz))  # warmup
    seen, windows, dt = 0, [], (Profile(), Profile(), Profile())
    for path, im, im0s, vid_cap, s in dataset:
        with dt[0]:
            im = torch.from_numpy(im).to(model.device)
            im = im.half() if model.fp16 else im.float()  # uint8 to fp16/32
            im /= 255  # 0 - 255 to 0.0 - 1.0
            if len(im.shape) == 3:
                im = im[None]  # expand for batch dim

        # Inference
        with dt[1]:
            visualize = increment_path(save_dir / Path(path).stem, mkdir=True) if visualize else False
            pred = model(im, augment=augment, visualize=visualize)

        # NMS
        with dt[2]:
            pred = non_max_suppression(pred, conf_thres, iou_thres, classes, agnostic_nms, max_det=max_det)

        # Second-stage classifier (optional)
        # pred = utils.general.apply_classifier(pred, classifier_model, im, im0s)

        # Process predictions
        for i, det in enumerate(pred):  # per image
            seen += 1
            if webcam:  # batch_size >= 1
                p, im0, frame = path[i], im0s[i].copy(), dataset.count
                s += f'{i}: '
            else:
                p, im0, frame = path, im0s.copy(), getattr(dataset, 'frame', 0)

            p = Path(p)  # to Path
            save_path = str(save_dir / p.name)  # im.jpg
            txt_path = str(save_dir / 'labels' / p.stem) + ('' if dataset.mode == 'image' else f'_{frame}')  # im.txt
            s += '%gx%g ' % im.shape[2:]  # print string
            gn = torch.tensor(im0.shape)[[1, 0, 1, 0]]  # normalization gain whwh
            imc = im0.copy() if save_crop else im0  # for save_crop
            annotator = Annotator(im0, line_width=line_thickness, example=str(names))
            if len(det):
                # Rescale boxes from img_size to im0 size
                det[:, :4] = scale_boxes(im.shape[2:], det[:, :4], im0.shape).round()

                # Print results
                for c in det[:, 5].unique():
                    n = (det[:, 5] == c).sum()  # detections per class
                    s += f"{n} {names[int(c)]}{'s' * (n > 1)}, "  # add to string

                # Write results
                for *xyxy, conf, cls in reversed(det):
                    if save_txt:  # Write to file
                        xywh = (xyxy2xywh(torch.tensor(xyxy).view(1, 4)) / gn).view(-1).tolist()  # normalized xywh
                        line = (cls, *xywh, conf) if save_conf else (cls, *xywh)  # label format
                        with open(f'{txt_path}.txt', 'a') as f:
                            f.write(('%g ' * len(line)).rstrip() % line + '\n')

                    if save_img or save_crop or view_img:  # Add bbox to image
                        c = int(cls)  # integer class
                        label = None if hide_labels else (names[c] if hide_conf else f'{names[c]} {conf:.2f}')
                        annotator.box_label(xyxy, label, color=colors(c, True))
                    if save_crop:
                        save_one_box(xyxy, imc, file=save_dir / 'crops' / names[c] / f'{p.stem}.jpg', BGR=True)

            # Stream results
            im0 = annotator.result()
            if view_img:
                if platform.system() == 'Linux' and p not in windows:
                    windows.append(p)
                    cv2.namedWindow(str(p), cv2.WINDOW_NORMAL | cv2.WINDOW_KEEPRATIO)  # allow window resize (Linux)
                    cv2.resizeWindow(str(p), im0.shape[1], im0.shape[0])
                cv2.imshow(str(p), im0)
                cv2.waitKey(1)  # 1 millisecond

            # Save results (image with detections)
            if save_img:
                if dataset.mode == 'image':
                    cv2.imwrite(save_path, im0)
                else:  # 'video' or 'stream'
                    if vid_path[i] != save_path:  # new video
                        vid_path[i] = save_path
                        if isinstance(vid_writer[i], cv2.VideoWriter):
                            vid_writer[i].release()  # release previous video writer
                        if vid_cap:  # video
                            fps = vid_cap.get(cv2.CAP_PROP_FPS)
                            w = int(vid_cap.get(cv2.CAP_PROP_FRAME_WIDTH))
                            h = int(vid_cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
                        else:  # stream
                            fps, w, h = 30, im0.shape[1], im0.shape[0]
                        save_path = str(Path(save_path).with_suffix('.mp4'))  # force *.mp4 suffix on results videos
                        vid_writer[i] = cv2.VideoWriter(save_path, cv2.VideoWriter_fourcc(*'mp4v'), fps, (w, h))
                    vid_writer[i].write(im0)

        # Print time (inference-only)
        LOGGER.info(f"{s}{'' if len(det) else '(no detections), '}{dt[1].dt * 1E3:.1f}ms")

    # Print results
    t = tuple(x.t / seen * 1E3 for x in dt)  # speeds per image
    LOGGER.info(f'Speed: %.1fms pre-process, %.1fms inference, %.1fms NMS per image at shape {(1, 3, *imgsz)}' % t)
    if save_txt or save_img:
        s = f"\n{len(list(save_dir.glob('labels/*.txt')))} labels saved to {save_dir / 'labels'}" if save_txt else ''
        LOGGER.info(f"Results saved to {colorstr('bold', save_dir)}{s}")
    if update:
        strip_optimizer(weights[0])  # update model (to fix SourceChangeWarning)

def parse_opt():
    parser = argparse.ArgumentParser()
    parser.add_argument('--weights', nargs='+', type=str, default= '/root/data/best.pt', help='model path or triton URL')
    parser.add_argument('--source', type=str, default= '/root/SaveFile/', help='file/dir/URL/glob/screen/0(webcam)')
    parser.add_argument('--data', type=str, default= '/root/data/data.yaml', help='(optional) dataset.yaml path')
    parser.add_argument('--imgsz', '--img', '--img-size', nargs='+', type=int, default=[640], help='inference size h,w')
    parser.add_argument('--conf-thres', type=float, default=0.25, help='confidence threshold')
    parser.add_argument('--iou-thres', type=float, default=0.45, help='NMS IoU threshold')
    parser.add_argument('--max-det', type=int, default=1000, help='maximum detections per image')
    parser.add_argument('--device', default='', help='cuda device, i.e. 0 or 0,1,2,3 or cpu')
    parser.add_argument('--view-img', action='store_true', help='show results')
    parser.add_argument('--save-txt', action='store_true', help='save results to *.txt')
    parser.add_argument('--save-conf', action='store_true', help='save confidences in --save-txt labels')
    parser.add_argument('--save-crop', action='store_true', help='save cropped prediction boxes')
    parser.add_argument('--nosave', action='store_true', help='do not save images/videos')
    parser.add_argument('--classes', nargs='+', type=int, help='filter by class: --classes 0, or --classes 0 2 3')
    parser.add_argument('--agnostic-nms', action='store_true', help='class-agnostic NMS')
    parser.add_argument('--augment', action='store_true', help='augmented inference')
    parser.add_argument('--visualize', action='store_true', help='visualize features')
    parser.add_argument('--update', action='store_true', help='update all models')
    parser.add_argument('--project', default= '/root/AItest', help='save results to project/name')
    parser.add_argument('--name', default= 'exp', help='save results to project/name')
    parser.add_argument('--exist-ok', action='store_true', help='existing project/name ok, do not increment')
    parser.add_argument('--line-thickness', default=3, type=int, help='bounding box thickness (pixels)')
    parser.add_argument('--hide-labels', default=False, action='store_true', help='hide labels')
    parser.add_argument('--hide-conf', default=False, action='store_true', help='hide confidences')
    parser.add_argument('--half', action='store_true', help='use FP16 half-precision inference')
    parser.add_argument('--dnn', action='store_true', help='use OpenCV DNN for ONNX inference')
    parser.add_argument('--vid-stride', type=int, default=1, help='video frame-rate stride')
    opt = parser.parse_args()
    opt.imgsz *= 2 if len(opt.imgsz) == 1 else 1  # expand
    print_args(vars(opt))
    return opt

def main(opt):
    check_requirements(exclude=('tensorboard', 'thop'))
    run(**vars(opt))

def runs():
    client = connect_mqtt()
    subscribe(client)
    client.loop_forever()

if __name__ =='__main__':
    runs()
    time.sleep(100)

   ```
